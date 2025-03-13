#include "ScoreComponent.h"

#include "Achievements.h"
#include "AchievementsGlobals.h"
#include "CSteamAchievement.h"

void dae::ScoreComponent::AddScore(int amount)
{
	m_score += amount;
	NotifyObservers(Event::PlayerDied, this);

	if (m_score >= 500 && g_SteamAchievements)
		g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
}

int dae::ScoreComponent::GetScore() const
{
	return m_score;
}
