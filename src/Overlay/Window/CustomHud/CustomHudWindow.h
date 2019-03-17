#pragma once
#include "Overlay/Window/Window.h"
#include "CharMetersWindow.h"
#include "HealthWindow.h"
#include "MetersWindow.h"
#include "TimerWindow.h"

#include "Core/interfaces.h"

class CustomHudWindow : public Window
{
public:
	CustomHudWindow(const std::string& windowTitle, bool windowClosable, ImGuiWindowFlags windowFlags);
	~CustomHudWindow() override = default;
	void SetScale(const ImVec2& scale);
	void SetWindowsMovable(bool isMainWindowVisible);
	void DrawResetWindowsPositionsButton();
protected:
	void Update() override;
	void Draw() override;

private:
	bool HasNullPointerInData() const;
	void UpdateHealthWindow(bool isPlayerTwo);
	void UpdateUniqueMeterWindow(bool isPlayerTwo);
	void SwapActive(const CharInfo** characterActive, const CharInfo** characterInactive) const;

	TimerWindow m_timerWindow = TimerWindow("TimerWindow", false, m_windowFlags);
	HealthWindow m_healthWindowLeft = HealthWindow("PlayerOneHealthWindow", false, m_windowFlags);
	HealthWindow m_healthWindowRight = HealthWindow("PlayerTwoHealthWindow", false, m_windowFlags);
	MetersWindow m_metersWindowLeft = MetersWindow("PlayerOneMetersWindow", false, m_windowFlags);
	MetersWindow m_metersWindowRight = MetersWindow("PlayerTwoMetersWindow", false, m_windowFlags);
	CharMetersWindow m_uniqueMetersWindowLeft = CharMetersWindow("PlayerOneUniqueMetersWindow", false, m_windowFlags);
	CharMetersWindow m_uniqueMetersWindowRight = CharMetersWindow("PlayerTwoUniqueMetersWindow", false, m_windowFlags);
};
