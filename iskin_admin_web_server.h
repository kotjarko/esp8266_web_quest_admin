#define DEBUG_SERIAL Serial1
#define WORK_SERIAL Serial

#define MAX_STATUS_LEN    	64
#define MAX_STATUS_ELEMS	8

// логин и пароль точки
const char *ssid = "esptest";
const char *password = "12345678";

// переменная для хранения полученных данных
String val = "";
//char val = 0;

char status_buf[MAX_STATUS_LEN];
short status_len;
bool status_overflow;

// запуск вебсервера на 80 порту
ESP8266WebServer server(80);
// имя cookie авторизации
const String session_name = "ESPSESSIONID";
// удалось или нет подключиться к СД
static bool hasSD = false;

String temp_string = "";

short led_activate=0;
unsigned long  led_start = 0;

unsigned long  led_duration = 50; //msec

long ping_interval = 1000;
long ping_timeout = 500;
long millis_ping_send = 0;
short ping_get = 1;
short ping_errors = 0;

short room1_comm = 0;
short room1_game = 0;
short room1_exit=0;

short room1_object_1=0;
short room1_object_2=0;
short room1_object_3=0;

short quest = 0;
String elapsed = "";

short printer = 0;
short lift = 0;
short extr = 0;
short shrk_platform = 0;
short shrk_door = 0;
short shrk_detect = 0;

short main_light = 0;
short uv_light = 0;
short num_objects=3;

short bgame = 0;
short scan = 0;

short room1_sound = 0;

///////////////////////////////////////////////
short room2_exitdoor = 0;
short room2_vdoor = 0;
short room2_vdoor_lock = 0;
short room2_main_light = 0;

short room2_mockup = 0;
short room2_table = 0;
short room2_ballgame = 0;

short room1_error_count = 0;
short room2mockup_error_count = 0;
short room2vdoor_error_count = 0;
short room2table_error_count = 0;

short room2_stand_coil1 = 0;
short room2_stand_coil2 = 0;
short room2_stand_coil3 = 0;
short room2_eyes_circuit = 0;
short room2_mockup_wall1 = 0;
short room2_mockup_wall2 = 0;
short room2_mockup_wall3 = 0;
short room2_mockup_wall4 = 0;
short room2_mockup_wall5 = 0;
short room2_mockup_wall6 = 0;
short room2_mockup_up_light = 0;
short room2_mockup_button_light = 0;
short room2_mockup_wall_light = 0;
short room2_shome_voice = 0;
short room2_safe = 0;
short room2_magbridge_circuit = 0;
short room2_table_coil1 = 0;
short room2_table_coil2 = 0;
short room2_table_coil3 = 0;
short room2_table_led1 = 0;
short room2_table_led2 = 0;
short room2_keyslot1 = 0;
short room2_vdoor_sensor_light = 0;

short room2_vdoor_sensor_detect = 0;

short room2_vdoor_sensor_data_R = 0;
short room2_vdoor_sensor_data_G = 0;
short room2_vdoor_sensor_data_B = 0;
short room2_vdoor_sensor_data_W = 0;

short room2_kid_voice = 0;
short room2_vinyl = 0;
short room2_vinyl_enable = 0;
short room2_floorlight_R = 0;
short room2_floorlight_G = 0;
short room2_floorlight_B = 0;

short room2_game = 0;
