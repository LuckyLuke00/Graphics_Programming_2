#pragma once

class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	bool m_ResetScene{ false };

	FixedCamera* m_pCamera{ nullptr };

	GameObject* m_pBall{ nullptr };

	GameObject* m_pLeftPaddle{ nullptr };
	GameObject* m_pRightPaddle{ nullptr };

	GameObject* m_pLeftTrigger{ nullptr };
	GameObject* m_pRightTrigger{ nullptr };

	void CreateBall(const PxMaterial* material);
	void CreateCamera();
	void CreatePaddles(const PxMaterial* material);
	void CreateTriggers(const PxMaterial* material);
	void CreateWalls(const PxMaterial* material);

	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);

	void ResetScene();
};
