// Fill out your copyright notice in the Description page of Project Settings.


#include "PSDataBaseMgr.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <jni.h>
#endif

#include "PSCharacterMgr.h"
#include "PSStageMgr.h"
#include "Auth/Credential.h"
#include "Auth/User.h"
#include "Database/Database.h"
#include "Database/DatabaseReference.h"
#include "Google/GoogleServices.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponDetailRecord.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"

void UPSDataBaseMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPSDataBaseMgr::Deinitialize()
{
	Super::Deinitialize();
}

void UPSDataBaseMgr::Load()
{
	Super::Load();


#if WITH_EDITOR
	//PCTestDatabase();
#endif
	
}

void UPSDataBaseMgr::Release()
{
	
	
	Super::Release();
}

void UPSDataBaseMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void UPSDataBaseMgr::GooglePlayLogin()
{
	m_pDatabase = UDatabase::GetInstance();

	if(m_pDatabase)
	{
		m_pDatabase->SetPersistenceEnabled(false);
#if PLATFORM_ANDROID
		UGoogleServicesLibrary::SignCallBack(FGoogleSignInCallback::CreateUObject(this, &ThisClass::OnTaskOver));
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Sign_In", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif

		
	}
}

void UPSDataBaseMgr::GetCharacterStat(UDataSnapshot* dataSnapshot)
{
	if(UDataSnapshot* CharacterStatSnapShot = dataSnapshot->GetChild(TEXT("CharacterStat")))
	{
		TArray<UDataSnapshot*> arrCharacterStat = CharacterStatSnapShot->GetChildren();

		for(UDataSnapshot* pSnapShot:arrCharacterStat)
		{
			g_PSCharacterMgr->m_MapCharacterLevel.Emplace(pSnapShot->GetKey(),pSnapShot->GetValue().AsInt64());
		}
		g_PSCharacterMgr->InitCharacterStat();
	}
	else
	{
		InitCharacterStat();
	}
}

void UPSDataBaseMgr::GetCharacterGoods(UDataSnapshot* dataSnapshot)
{
	if(UDataSnapshot* CharacterStatSnapShot = dataSnapshot->GetChild(TEXT("CharacterGoods")))
	{
		g_PSCharacterMgr->InitCharacterGoods(CharacterStatSnapShot);
	}
	else
	{
		InitCharacterGoods();
	}
}

void UPSDataBaseMgr::GetWeaponDetail(UDataSnapshot* DataSnapshot)
{
	if(UDataSnapshot* WeaponDetailSnapShot = DataSnapshot->GetChild(TEXT("WeaponDetail")))
	{
		TArray<UDataSnapshot*> arrWeaponDetail = WeaponDetailSnapShot->GetChildren();

		if(arrWeaponDetail.IsEmpty())
		{
			InitWeaponDetail();
		}
		int32 Number= 0;
		g_PSCharacterMgr->m_mapEquipWeapon.Empty();
		for(UDataSnapshot* pSnapShot:arrWeaponDetail)
		{
			FString tid = pSnapShot->GetKey();
			int32 Level = pSnapShot->GetChild(TEXT("Level"))->GetValue().AsInt32();
			bool Equip = pSnapShot->GetChild(TEXT("Equip"))->GetValue().AsBool();
			int32 Piece = pSnapShot->GetChild(TEXT("Piece"))->GetValue().AsInt32();
			if(g_PSCharacterMgr->m_MapWeapon.Contains(tid))
			{
				g_PSCharacterMgr->m_MapWeapon[tid]->m_IsLock = true;
				g_PSCharacterMgr->m_MapWeapon[tid]->Level = Level;
				g_PSCharacterMgr->m_MapWeapon[tid]->m_IsEquip = Equip;
				g_PSCharacterMgr->m_MapWeapon[tid]->Piece = Piece;
				if(Equip)
				{
					g_PSCharacterMgr->m_mapEquipWeapon.Emplace(Number,g_PSCharacterMgr->m_MapWeapon[tid]);
					Number++;
				}
				g_PSCharacterMgr->OnRefreshWeapon();


			}
		}
	
	}
	else
	{
		InitWeaponDetail();
	}
}

void UPSDataBaseMgr::GetStageInfo(UDataSnapshot* DataSnapshot)
{
	if(UDataSnapshot* StageInfoSnapShot = DataSnapshot->GetChild(TEXT("StageInfo")))
	{
		int32 MaxStage = StageInfoSnapShot->GetChild(TEXT("MaxStage"))->GetValue().AsInt32();
		
		if(g_StageMgrValid)
		{
			MaxStage == 0 ? g_StageMgr->SetMaxStageNumber(1) : g_StageMgr->SetMaxStageNumber(MaxStage);
			g_StageMgr->SetStageNumber(g_StageMgr->GetMaxStageNumber());
			
			if(UDataSnapshot* StageSnapshot = StageInfoSnapShot->GetChild(TEXT("Stage")))
			{
				TArray<UDataSnapshot*> arrStageSnapshot = StageSnapshot->GetChildren();
				if(arrStageSnapshot.IsEmpty())
				{
					InitStageInfo();
					return;
				}

				for (int32 stageIndex = 0; stageIndex < arrStageSnapshot.Num(); stageIndex++)
				{
					FString StageName = arrStageSnapshot[stageIndex]->GetKey();
					int32 hightWave = arrStageSnapshot[stageIndex]->GetValue().AsInt32();

					g_StageMgr->m_arrStageHighWave.Emplace(StageName,hightWave);
				}
			}
		}
	}
	else
	{
		InitStageInfo();
	}
}

void UPSDataBaseMgr::SetCharacterStats(const FString& _statName, int32 _level)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/CharacterStat/") + _statName;

		FFirebaseVariant varientValue = _level;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetCharacterGoods(const FString& _statName, int32 _level)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/CharacterGoods/") + _statName;

		FFirebaseVariant varientValue = _level;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetCharacterGoodsString(const FString& _statName, const FString& _value)
{
	if (m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/CharacterGoods/") + _statName;

		FFirebaseVariant varientValue = _value;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue, FDatabaseCallback::CreateUObject(this, &UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetWeaponEquip(const FString& _weaponName, bool _isEquip)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/WeaponDetail/") + _weaponName.ToLower() + ("/Equip");


		FFirebaseVariant varientValue = _isEquip;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetWeaponLevel(const FString& _weaponName, int32 _level,int32 piece)
{
	if(m_pDatabase)
	{
		m_strWeaponTid = _weaponName.ToLower();
		m_Levelup = _level;
		m_Piece = piece;
		FString strPath = m_strUserUID + TEXT("/WeaponDetail/") + _weaponName.ToLower();
		TMap<FFirebaseVariant,FFirebaseVariant> mapWeaponLevel;

		mapWeaponLevel.Emplace("Level",_level);
		mapWeaponLevel.Emplace("Piece",piece);
		FFirebaseVariant varientValue = mapWeaponLevel;
		m_pDatabase->GetReferenceFromPath(strPath)->UpdateChildren(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverWeaponLevelup));
	}
}

void UPSDataBaseMgr::SetWeaponPiece(const FString& _weaponName, int32 Piece)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/WeaponDetail/") + _weaponName.ToLower() ;
		TMap<FFirebaseVariant,FFirebaseVariant> mapWeaponLevel;
		mapWeaponLevel.Emplace("Piece",Piece);
		FFirebaseVariant varientValue = mapWeaponLevel;
		m_pDatabase->GetReferenceFromPath(strPath)->UpdateChildren(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetWeaponUnLock(const FString& _weaponName)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/WeaponDetail/") + _weaponName.ToLower();
		TMap<FFirebaseVariant,FFirebaseVariant> mapUnlock;
		mapUnlock.Emplace("Equip",false);
		mapUnlock.Emplace("Level",1);
		mapUnlock.Emplace("Piece",0);
		FFirebaseVariant varientValue = mapUnlock;
		m_pDatabase->GetReferenceFromPath(strPath)->UpdateChildren(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::SetStageInfo(int32 maxStage)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/StageInfo");
		TMap<FFirebaseVariant,FFirebaseVariant> mapMaxStage;
		mapMaxStage.Emplace("MaxStage",maxStage);
		FFirebaseVariant varientValue = mapMaxStage;
		m_pDatabase->GetReferenceFromPath(strPath)->UpdateChildren(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));


	}
}

void UPSDataBaseMgr::SetStageHighWave(int32 _stageNumber, int32 _highWave)
{
	if(m_pDatabase)
	{
		FString strTid = TEXT("Stage_")+FString::FromInt(_stageNumber);
		FString strPath = m_strUserUID + TEXT("/StageInfo/Stage/")+strTid;
		FFirebaseVariant varientValue = _highWave;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::InitCharacterStat()
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/CharacterStat");

		TMap<FFirebaseVariant,FFirebaseVariant> statInit;
		statInit.Emplace("Armor",1);
		statInit.Emplace("CoolDown",1);
		statInit.Emplace("Damage",1);
		statInit.Emplace("Health",1);
		statInit.Emplace("MonsterAttack",1);
		statInit.Emplace("MonsterMove",1);

		FFirebaseVariant PushValue = statInit;

		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::InitCharacterGoods()
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/CharacterGoods");
		
		FDateTime CurrentUtcTime = FDateTime::UtcNow();
		FDateTime CurrentKstTime = CurrentUtcTime + FTimespan(0, 9, 0, 0);
		FString InitialLastSavedTime = CurrentKstTime.ToString();
		
		TMap<FFirebaseVariant,FFirebaseVariant> goodsInit;
		goodsInit.Emplace("Dia",0);
		goodsInit.Emplace("Energy",30);
		goodsInit.Emplace("Gold",0);
		goodsInit.Emplace("LastSavedTime", InitialLastSavedTime);

		FFirebaseVariant PushValue = goodsInit;

		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));

		
		if (g_PSCharacterMgrValid)
		{
			int InitEnergy = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_ENERGY_END");
			g_PSCharacterMgr->SetEnergy(InitEnergy);
		}
	}
}

void UPSDataBaseMgr::InitWeaponDetail()
{
	TArray<PSWeaponDetailRecord*> arrWeaponDetails;
	g_WeaponDetailRecordMgr->GetAllRecord(arrWeaponDetails);

	for(PSWeaponDetailRecord* pRecord : arrWeaponDetails)
	{
		if(pRecord->m_WeaponStat->m_IsLock)
		{
			if(m_pDatabase)
			{
				TMap<FFirebaseVariant,FFirebaseVariant> weaponDetailInit;
				FString strPath = m_strUserUID + TEXT("/WeaponDetail/") + pRecord->m_strTid.ToString().ToLower();
				FString strLevel =  TEXT("Level");
				FString strPiece =  TEXT("Piece");
				FString strEquip = TEXT("Equip");
				weaponDetailInit.Emplace(strLevel,1);
				weaponDetailInit.Emplace(strPiece,0);
				weaponDetailInit.Emplace(strEquip,true);
				FFirebaseVariant PushValue = weaponDetailInit;

				m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
			}
		}
	}

}

void UPSDataBaseMgr::InitStageInfo()
{




			if(m_pDatabase)
			{

				
				TMap<FFirebaseVariant,FFirebaseVariant> mapStageInfo;
				TMap<FFirebaseVariant,FFirebaseVariant> mapStage;
				FString strPath = m_strUserUID + TEXT("/StageInfo");
				FString strMaxStage =  TEXT("MaxStage");
				FString strStage =  TEXT("Stage_1");
				g_StageMgr->m_arrStageHighWave.Emplace(strStage,0);
				g_StageMgr->SetMaxStageNumber(1);
				mapStageInfo.Emplace(strMaxStage,1);
				
				FFirebaseVariant PushValue = mapStageInfo;
				m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
				
				FString strStagePath = m_strUserUID + TEXT("/StageInfo/Stage/Stage_1");
				FFirebaseVariant varientValue = 0;
				m_pDatabase->GetReferenceFromPath(strStagePath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
			}
		
	
}

void UPSDataBaseMgr::GetPurchaseInfo(UDataSnapshot* dataSnapshot)
{
	if(UDataSnapshot* CharacterStatSnapShot = dataSnapshot->GetChild(TEXT("PurchaseInfo")))
	{
		g_PSCharacterMgr->SetPurchasedAdRemove(CharacterStatSnapShot->GetChild("AdRemove")->GetValue().AsBool());
	}
	else
	{
		InitPurchaseInfo();
	}
}

void UPSDataBaseMgr::SetPurchaseInfo(const FString& _statName, bool _purchased)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/PurchaseInfo/") + _statName;

		FFirebaseVariant varientValue = _purchased;
		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::InitPurchaseInfo()
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/PurchaseInfo");

		TMap<FFirebaseVariant,FFirebaseVariant> goodsInit;
		goodsInit.Emplace("AdRemove",false);

		FFirebaseVariant PushValue = goodsInit;

		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::InitStoreInfo()
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/StoreInfo");

		TMap<FFirebaseVariant,FFirebaseVariant> storeInit;
		storeInit.Emplace("FreeGold",false);

		FFirebaseVariant PushValue = storeInit;

		m_pDatabase->GetReferenceFromPath(strPath)->SetValue(PushValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}

void UPSDataBaseMgr::GetStoreInfo(UDataSnapshot* dataSnapshot)
{
	if(UDataSnapshot* CharacterStatSnapShot = dataSnapshot->GetChild(TEXT("StoreInfo")))
	{
		if(CharacterStatSnapShot->GetChild("FreeGold")->GetValue().IsNull())
		{
			InitStoreInfo();
		}
		else
		{
			g_PSCharacterMgr->SetDisableFreeGold(CharacterStatSnapShot->GetChild("FreeGold")->GetValue().AsBool());
		}

	}
	else
	{
		InitStoreInfo();
	}
}

void UPSDataBaseMgr::SetStoreInfo(const FString& storelistName, bool _enable)
{
	if(m_pDatabase)
	{
		FString strPath = m_strUserUID + TEXT("/StoreInfo/") + storelistName;

		FFirebaseVariant varientValue = _enable;
		m_pDatabase->GetReferenceFromPath(strPath)->UpdateChildren(varientValue,FDatabaseCallback::CreateUObject(this,&UPSDataBaseMgr::OnOverSetStat));
	}
}


void UPSDataBaseMgr::SnapShotActionOver(EFirebaseDatabaseError FirebaseDatabaseError, UDataSnapshot* DataSnapshot)
{
	UE_LOG(LogTemp,Warning,TEXT("%d"),FirebaseDatabaseError)
	if(DataSnapshot)
	{
		if(DataSnapshot->GetChildren().Num() > 0)
		{
			GetCharacterStat(DataSnapshot);
			GetCharacterGoods(DataSnapshot);
			GetWeaponDetail(DataSnapshot);
			GetStageInfo(DataSnapshot);
			GetPurchaseInfo(DataSnapshot);
			GetStoreInfo(DataSnapshot);
			
			FString LastSavedTime;
			if (UDataSnapshot* LastSavedTimeSnapshot = DataSnapshot->GetChild(TEXT("CharacterGoods/LastSavedTime")))
			{
				LastSavedTime = LastSavedTimeSnapshot->GetValue().AsString();
				if (g_PSCharacterMgrValid)
				{
					g_PSCharacterMgr->SetLastSavedTime(LastSavedTime);
				}
			}
		}
		else
		{
			InitCharacterStat();
			InitCharacterGoods();
			InitWeaponDetail();
			InitStageInfo();
			InitPurchaseInfo();
			InitStoreInfo();
		}
		m_IsLoad = true;
		if(LoadCompleteDataBase.IsBound())
		{
			LoadCompleteDataBase.Broadcast();
		}
	}

}

void UPSDataBaseMgr::OnActionOver(EFirebaseAuthError FirebaseAuthError, UUser* User)
{
	if(User)
	{
		m_strUserUID = User->Uid();
		m_strUserName = User->DisplayName();
		m_strPhotoURL = User->PhotoUrl();
		m_bGoogleEnable = true;


	}
	else
	{
		// if(g_GameMsgMgrValid)
		// {
		// 	g_GameMsgMgr->ShowMessageBox("Popup_FailLogin",this,"OpenGameLevel");
		// }

		GoogldLoginFailed();
		m_bGoogleEnable = false;


		return ;
	}
	m_pDatabase = UDatabase::GetInstance();

	FString strPath = m_strUserUID;;

	if(m_pDatabase)
	{
		m_pDatabase->GetReferenceFromPath(strPath)->GetValue(FSnapshotCallback::CreateUObject(this,&ThisClass::SnapShotActionOver));
	}
}

void UPSDataBaseMgr::OnTaskOver(bool bArg, FString String)
{
	if (bArg)
	{
		// Standard Google Sign-In.
		if (!bUsePlayGames)
		{
			FString IdToken     = UGoogleServicesLibrary::GetIdToken();
			FString AccessToken = UGoogleServicesLibrary::GetAccessToken();
			


			Credential = UCredentialLibrary::GetCredentialFromGoogle(IdToken, AccessToken);
			FAuth::SignInWithCredential(Credential, FSignInUserCallback::CreateUObject(this, &ThisClass::OnActionOver));
			
		}
		// Google Play Games Sign-In
		else
		{
			FString ServerAuthCode = UGoogleServicesLibrary::GetServerAuthCode();

			UE_LOG(LogTemp,Warning,TEXT("ServerAuthCode %s"),*ServerAuthCode)
			
			if (ServerAuthCode.IsEmpty())
			{
	
				GoogldLoginFailed();
			}
			else
			{
				Credential = UCredentialLibrary::GetCredentialFromPlayGames(ServerAuthCode);

				UE_LOG(LogTemp,Warning,TEXT("UserName %s"),*Credential.Provider());
				FAuth::SignInWithCredential(Credential, FSignInUserCallback::CreateUObject(this, &ThisClass::OnActionOver));
			}
		}
	}
	else
	{

		
		m_bGoogleEnable = false;

		GoogldLoginFailedTo();
	}
}

void UPSDataBaseMgr::OnOverSetStat(EFirebaseDatabaseError FirebaseDatabaseError)
{
	if(FirebaseDatabaseError == EFirebaseDatabaseError::None)
	{
		UE_LOG(LogTemp,Warning,TEXT("Complete Value"));

		OnOverWeaponLevelup(FirebaseDatabaseError);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Error!!! %d"),FirebaseDatabaseError);
	}
}

void UPSDataBaseMgr::OnOverWeaponLevelup(EFirebaseDatabaseError FirebaseDatabaseError)
{
	if(FirebaseDatabaseError == EFirebaseDatabaseError::None)
	{
		UE_LOG(LogTemp,Warning,TEXT("Complete Value"));

		if(WeaponLevelup.IsBound())
		{
			WeaponLevelup.Broadcast(m_strWeaponTid,m_Levelup,m_Piece);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Error!!! %d"),FirebaseDatabaseError);
	}
}

void UPSDataBaseMgr::GoogldLoginFailed()
{
	UE_LOG(LogTemp,Warning,TEXT("구글 로그인 실패 !! 다시 접속 바람"))

	if(g_GameMsgMgrValid)
	{
		g_GameMsgMgr->ShowMessageBox("Popup_Not_GooglePlay",this,TEXT("OnFailedConnectFirebase"));
	}
}

void UPSDataBaseMgr::GoogldLoginFailedTo()
{
	UE_LOG(LogTemp,Warning,TEXT("첫번 째 구글 로그인 실패 !! 다시 접속 바람"))

	if(g_GameMsgMgrValid)
	{
		g_GameMsgMgr->ShowMessageBox("Popup_Not_GooglePlay",this,TEXT("OnFailedConnectFirebase"));
	}
}

void UPSDataBaseMgr::OnFailedConnectFirebase()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit,false);
}


void UPSDataBaseMgr::PCTestDatabase()
{
	m_pDatabase = UDatabase::GetInstance();

	FString strPath = TEXT("Test2");
	m_strUserUID = strPath;
	if(m_pDatabase)
	{
		m_pDatabase->SetPersistenceEnabled(false);
		if(UDatabaseReference* pRefrence = m_pDatabase->GetReference())
		{
			pRefrence->SetKeepSynchronized(true);
			pRefrence->Child(strPath)->GetValue(FSnapshotCallback::CreateUObject(this,&ThisClass::SnapShotActionOver));
		}
	}
}
