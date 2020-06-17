#pragma once

struct sprite
{
	ID3D11ShaderResourceView* texture = NULL;
	float width =0.0f;
	float height = 0.0f;
	D3DXVECTOR2 pivot = D3DXVECTOR2(0.0f,0.0f);
	D3DXVECTOR2 offset = D3DXVECTOR2(0.0f, 0.0f);
};
struct Animation
{
	bool isLoop = false;
	vector<float> frame;
	vector<sprite*> texture;
};
class Animator;
class ImageManager
{
public:
	ImageManager();
	~ImageManager();
	static ImageManager & inse() { static ImageManager instance; return instance; };
public:
	void Init();
	void Shutdown();
	HRESULT LoadImagePack(int PackID);
	sprite* GetSceneSprite(string name);
	sprite* GetSeriaSprite(string name);
	sprite* GetUISprite(string name);
	Animation * GetBgAnimation(string name);
	Animator GetAnimator(string name);
private:
	void AddSprite(unordered_map<string, sprite*> & pack, string name, string path, float width, float height);
	void AddSprite(unordered_map<string, sprite*> & pack, string name, string path, float width, float height, D3DXVECTOR2 offset);
	void AddSprite(unordered_map<string, sprite*> & pack, string name, string path, float width, float height, D3DXVECTOR2 pivot, D3DXVECTOR2 offset);
	void AddCharacterAnimationMaker(Animation* anime, string path, float width, float height, D3DXVECTOR2 pivot,D3DXVECTOR2 offset, float frame);
	void AddBackGroundAnimationMaker(Animation* anime, string path, float width, float height, D3DXVECTOR2 pivot, D3DXVECTOR2 offset, float frame);

	void LoadBelMyrePrincipality_South();
	void LoadPlayableCharacterPack();
	void LoadSeriaRoom();
	void LoadUI();
private:
	int _currentPackID; // 로드된 이미지 백 id(큰범위 마을 
	unordered_map<string, sprite *> _mSceneSprites;
	unordered_map<string, sprite *> _mUISprites;
	unordered_map<string, sprite*> _mSeriaSprites;
	unordered_map<string, sprite*> _mCharacters;
	
	unordered_map<string, Animator> _mAnimatorControlls;
	unordered_map<string, Animation *> _mBackgroundGifs;//sprite animation
};

