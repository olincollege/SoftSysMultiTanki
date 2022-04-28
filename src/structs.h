typedef struct Texture Texture;
typedef struct Player Player;
typedef struct Bullet Bullet;
typedef struct MapTile MapTile;
typedef struct Effect Effect;

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

struct Texture {
	char name[MAX_NAME_LENGTH];
	SDL_Texture *texture;
	Texture *next;
};

typedef struct {
	int x;
	int y;
	int button[MAX_MOUSE_BUTTONS];
	int wheel;
} Mouse;

struct Effect {
	float x;
	float y;
	int active;
	int maxHealth;
	int health;
	int angle;
	SDL_Texture *texture;
	Effect *next;
};

struct Bullet {
	int playerIndex;
	int active;
	int isDead;
	float x;
	float y;
	int w;
	int h;
	int health;
	float dx;
	float dy;
	int angle;
	float bp[2];
	Effect etrailHead, *etrailTail;
	float trailDistance;
	SDL_Texture *texture;
	Bullet *next;
};

struct Player {
	int playerIndex;
	int isBody;
	int isDead;
	float x;
	float y;
	int w;
	int h;
	int health;
	int reload;
	int ammo;
	float dx;
	float dy;
	int angle;
	Effect etrailHead, *etrailTail;
	Effect bulletShot;
	float trailDistance;
	SDL_Texture *texture;
	Bullet bHead, *bTail;
	Player *next;
};

struct MapTile {
	float x;
	float y;
	SDL_Texture *texture;
	MapTile *next;
};

typedef struct {
	int keyboard[MAX_KEYBOARD_KEYS];
	Mouse mouse;
	int testInt;
} PlayerInput;

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	int maxPlayer;
	int playerIndex;
	PlayerInput playerInputs[MAX_PLAYER];
	Texture textureHead, *textureTail;
	float fontScale;
} App;

typedef struct {
	Player pHead, *pTail;
	MapTile oHead, *oTail;
	MapTile* ground[GRID_HEIGHT][GRID_WIDTH];
} Stage;

