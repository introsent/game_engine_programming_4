#pragma once

#define _ACH_ID(id, name) { id, #id, name, "", false, 0 }

class CSteamAchievements;

struct Achievement_t
{
    int m_eAchievementID;
    const char* m_pchAchievementID;
    char m_rgchName[128];
    char m_rgchDescription[256];
    bool m_bAchieved;
    int m_iIconImage;
};

