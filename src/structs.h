typedef struct Texture Texture;
typedef struct Player Player;
typedef struct Bullet Bullet;
typedef struct MapTile MapTile;
typedef struct Effect Effect;
typedef struct KeyInfo KeyInfo;

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
	int health;
	float dx;
	float dy;
	int angle;

	int w;
	int h;
	float bp[2];
	Effect etrailHead, *etrailTail;
	float trailDistance;
	SDL_Texture *texture;
	Bullet *next;
};

struct Player {
	int isDead;
	float x;
	float y;
	int health;
	float dx;
	float dy;
	int angle;

	Bullet bHead, *bTail;

	int playerIndex;
	int isBody;
	int w;
	int h;
	int reload;
	int ammo;
	Effect etrailHead, *etrailTail;
	Effect bulletShot;
	float trailDistance;
	float trailSound;
	SDL_Texture *texture;
	Player *next;
};

struct MultiplayerInfo{
	int tank_no;
	char peer_ip_addr[16];
};

struct KeyInfo {
    uint8_t w_key_data;
	uint8_t a_key_data;
	uint8_t s_key_data;
	uint8_t d_key_data;
    uint8_t mouse_button_data;
    int32_t mouse_state_x_data;
    int32_t mouse_state_y_data;
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
	int AImove;
	int AIaim;
	int AItarget;
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
	int isMulti;
} App;

typedef struct {
	Player pHead, *pTail;
	MapTile oHead, *oTail;
	MapTile* ground[GRID_HEIGHT][GRID_WIDTH];
} Stage;

