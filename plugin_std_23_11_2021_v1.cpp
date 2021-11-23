struct PVec2f {
	union {
		struct {
			float x;
			float y;
		};

		float data[2];
	};

    PVec2f(float x, float y) {
        content[0] = x;
        content[1] = y;
    }

    inline float len_squared() const {
        return x * x + y * y;
    }

    inline float len() const {
        return sqrt(len_squared());
    }

    inline void clamp(const PVec2f &vmin, const PVec2f &vmax) {
        for (int coord = 0; coord < 2; ++coord) {
            content[coord] = fmin(fmax(content[coord], vmin.content[coord]), vmax.content[coord]);
        }
    }
};

union PRGBA {
	unsigned char rgba[4];
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
        unsigned char a;
	};
	int i;
	uint32_t ui32;

	inline bool is_empty() {
		return ui32 == 0;
	}

	PRGBA();
	PRGBA(const int x);
	PRGBA(const uint32_t x);
	PRGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_ = 255);

	PRGBA operator!() const; // negative

	PRGBA operator+(const PRGBA &other) const;
	PRGBA operator-(const PRGBA &other) const;

	PRGBA operator/(float coef) const;
	PRGBA operator*(float coef) const;
};


enum PluginType {
	PPT_EFFECT,
	PPT_TOOL,
};

enum PMouseButton {
	PMB_LEFT,
	PMB_RIGHT,
	PMB_MIDDLE,
};

enum PreviewLayerPolicy {
	PPLP_BLEND,
	PPLP_COPY,
};

enum PBlendMode {
	PPBM_COPY,
	PPBM_ALPHA_BLEND,
};

enum PDrawPolicy {
	PPDP_PREVIEW,
	PPDP_ACTIVE,
};

struct PRenderMode {
	PBlendMode blend;
	PDrawPolicy draw_policy;
};

struct PExtensionalInterface {
	bool (*has_func)(const char *name); // returns true if ext. func with given name exists
	void *(*execute)(const char *name, void *args); // runs given func with given args, interprenting them freely
};


struct PluginInterface;

struct PluginInfo {
	uint32_t std_version;
	void *reserved;

	PluginInterface *interface;

	const char *name;
	const char *version;
	const char *author;
	const char *description;

	PluginType type;
};


struct PluginInterface {
	uint32_t std_version;
	void *reserved;

	PExtensionalInterface extension;

	struct {
		const PluginInfo *(*get_info)();
		PluginStatus (*init)(const AppInterface*);
		PluginStatus (*deinit)();
		void (*dump)();

		void (*on_tick)(float dt);

		PreviewLayerPolicy (*get_flush_policy)();
	} general;

	union {
		struct {
			void (*apply)();
		} effect;

		struct {
			void (*on_press)(PVec2f);
			void (*on_release)(PVec2f);
			void (*on_move)(PVec2f);
		} tool;
	}
};


struct AppInterface {
	uint32_t std_version;
	void *reserved;

	PExtensionalInterface extension;

	struct {
		void (*log)(const char *, ...);
		float (*get_absolute_time)();
	} general;
	
	PRGBA *(*get_pixels)();
	void get_layer_size(size_t *width, size_t *height);

	struct {
		void (*circle)(PVec2f, float, PRGBA, *PRenderMode);
		void (*line)(PVec2f, PVec2f, PRGBA, *PRenderMode);
		void (*triangle)(PVec2f, PVec2f, PVec2f, float, PRGBA, *PRenderMode);
		void (*rectangle)(PVec2f, PVec2f, float, PRGBA, *PRenderMode);

		void (*pixels)(PRGBA *data, size_t width, size_t height, *PRenderMode);

	} render;

	// set everything to nullptr here if you don't support shaders
	struct {
		void (*apply)(void *shader, *PRenderMode);
		void *(*compile)(const char *code);

		void (*set_uniform)(const char *name, int  val);
		void (*set_uniform)(const char *name, int *val, size_t cnt);

		void (*set_uniform)(const char *name, float  val);
		void (*set_uniform)(const char *name, float *val, size_t cnt);

		void (*set_uniform)(const char *name, void *texture);
	} shader;
};
