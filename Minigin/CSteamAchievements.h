#pragma once
#include "Achievements.h"
#include <steam_api.h>

class CSteamAchievements final
{
public:
    CSteamAchievements(Achievement_t* pAchievements, int numAchievements);
    ~CSteamAchievements();
    CSteamAchievements(const CSteamAchievements&) = delete;             
    CSteamAchievements& operator=(const CSteamAchievements&) = delete;  
    CSteamAchievements(CSteamAchievements&&) = delete;                 
    CSteamAchievements& operator=(CSteamAchievements&&) = delete;

    bool static RequestStats();

    bool SetAchievement(const char* id) const;

private:
    int64 m_iAppID;           
    Achievement_t* m_pAchievements; 
    int m_iNumAchievements;    
    bool m_bInitialized;          

    // Steam callback members:
    STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
    STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
    STEAM_CALLBACK(CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};

