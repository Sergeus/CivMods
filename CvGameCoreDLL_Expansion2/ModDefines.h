
#ifdef ENABLE_CUSTOM_NOTIFICATIONS
	#define CUSTOM_NOTIFICATIONS 1
#else
	#define CUSTOM_NOTIFICATIONS 0
#endif // ENABLE_CUSTOM_NOTIFICATIONS

#ifdef ENABLE_CUSTOM_MISSIONS
	#define CUSTOM_MISSIONS 1
#else
	#define CUSTOM_MISSIONS 0
#endif // CUSTOM_MISSIONS

#ifdef ENABLE_SIEGEMOD
	#define SIEGEMOD 1
	#define CUSTOM_NOTIFICATIONS 1
	#define CUSTOM_MISSIONS 1
#else
	#define SIEGEMOD 0
	#define CUSTOM_NOTIFICATIONS 0
	#define CUSTOM_MISSIONS 0
#endif // ENABLE_SIEGEMOD

