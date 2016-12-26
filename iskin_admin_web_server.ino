#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

#include "iskin_admin_web_server.h" // переменные и константы
#include "iskin_auth.h" // функции авторизации
#include "iskin_page_handlers.h" // обработчики страниц
#include "iskin_sd.h" // работа с флешкой
#include "iskin_status.h" // статус - функция вывода для аякс

void set_handlers() {
  // Связываем страницы и их функции-обработчик
  if (!hasSD) {
    server.on("/", handle_root);
    server.onNotFound(handle_404);

    server.on("/login_form", handle_login_form);
    server.on("/secure/index.htm", handle_access);
  }
  else {
    server.on("/", handle_root_SD);
    server.onNotFound(handle_404_SD);
  }
}

void setup() {
  pinMode(5, OUTPUT);
  pinMode(16, INPUT);
  delay(3500);
  // инициализация UART
  WORK_SERIAL.begin(115200);
  DEBUG_SERIAL.begin(115200);

  // проверяем - подключена ли флешка
  if (SD.begin(SS, SPI_FULL_SPEED)) {
    hasSD = true;
  }

  // создаем точку WiFi
  WiFi.softAP("ISKINquest1", "12345678"); // ,1,1
  WiFi.mode(WIFI_AP_STA);
  // подключаемся к существующей точке
  WiFi.begin("Brainy", "123456789qwertyPO");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_SERIAL.print(".");
  }
  WORK_SERIAL.println(WiFi.localIP());
  
  // Связываем страницы и их функции-обработчики
  set_handlers();
  server.on("/login", handle_login);
  server.on("/serial", handle_serial_send);
  server.on("/status", handle_status);
  server.on("/reset_cpu", handle_cpu_reset);

  // выбираем поля заголовка, которые будут использоваться нами в программе
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize );
  // открываем веб-сервер на http://192.168.4.1
  server.begin();


}

void process_cpu_status(char *name, short argc, char **argv)
{
  //short i;
  //char buf[64];
  
  //WORK_SERIAL.print("command: ");
  //WORK_SERIAL.println(name);
  /*for(i = 0; i < argc; i++)
  {
    sprintf(buf, "arg%d: %s", i, argv[i]);
    WORK_SERIAL.println(buf);
  }*/
  
    if (strncmp(name, "room1", 5) == 0)
	{
      if (strcmp(name, "room1_game_stopped") == 0) room1_game = 0;
      else if (strcmp(name, "room1_game_wait_scan") == 0) room1_game = 1;
      else if (strcmp(name, "room1_game_wait_print_start") == 0) room1_game = 2;
      else if (strcmp(name, "room1_game_printing") == 0) room1_game = 3;
      else if (strcmp(name, "room1_game_bgame_wait") == 0) room1_game = 4;
      else if (strcmp(name, "room1_game_bgame_1") == 0) room1_game = 5;
      else if (strcmp(name, "room1_game_bgame_2") == 0) room1_game = 6;
      else if (strcmp(name, "room1_game_bgame_3") == 0) room1_game = 7;
      else if (strcmp(name, "room1_game_wait_shrk_activate") == 0) room1_game = 8;
      else if (strcmp(name, "room1_game_wait_shrk_object") == 0) room1_game = 9;
      else if (strcmp(name, "room1_game_wait_shrk_door") == 0) room1_game = 10;
      else if (strcmp(name, "room1_game_shrinking_object") == 0) room1_game = 11;
      else if (strcmp(name, "room1_game_wait_objects") == 0) room1_game = 12;
      else if (strcmp(name, "room1_game_ended") == 0) room1_game = 13;
  
  
      else if (strcmp(name, "room1_exit_unlocked") == 0) room1_exit = 0;
      else if (strcmp(name, "room1_exit_locked") == 0) room1_exit = 1;
  
      else if (strcmp(name, "room1_no_object_1") == 0) room1_object_1 = 0;
      else if (strcmp(name, "room1_have_object_1") == 0) room1_object_1 = 1;
      
      else if (strcmp(name, "room1_no_object_2") == 0) room1_object_2 = 0;
      else if (strcmp(name, "room1_have_object_2") == 0) room1_object_2 = 1;
      
      else if (strcmp(name, "room1_no_object_3") == 0) room1_object_3 = 0;
      else if (strcmp(name, "room1_have_object_3") == 0) room1_object_3 = 1;
  
      else if (strcmp(name, "room1_offline") == 0) room1_comm = 0;
      else if (strcmp(name, "room1_online") == 0) room1_comm = 1;
      else if (strcmp(name, "room1_comm_error") == 0) room1_comm = 2;
  
      else if (strcmp(name, "room1_game_num_objects") == 0)
	  {
        num_objects = atoi(argv[0]);
        DEBUG_SERIAL.println(temp_string);
      }
      
      else if( (strcmp(name, "room1_main_light") == 0) && (argc >= 1) )
	  {
        main_light = atoi(argv[0]);
        DEBUG_SERIAL.println(temp_string);     
      }
      
      else if (strcmp(name, "room1_uv_light") == 0)
	  {
        uv_light = atoi(argv[0]);
        DEBUG_SERIAL.println(temp_string);  
      }
      
      else if (strcmp(name, "room1_scan_default") == 0) scan = 0;
      else if (strcmp(name, "room1_scan_passed") == 0) scan = 1;
      
      else if (strcmp(name, "room1_printer_stopped") == 0) printer = 0;
      else if (strcmp(name, "room1_printer_active") == 0) printer = 1;
    
      else if (strcmp(name, "room1_lift_stopped") == 0) lift = 0;
      else if (strcmp(name, "room1_lift_moving") == 0) lift = 1;
      else if (strcmp(name, "room1_lift_error") == 0) lift = 2;
      else if (strcmp(name, "room1_lift_extended") == 0) lift = 3;
      else if (strcmp(name, "room1_lift_retracted") == 0) lift = 4;
  
      else if (strcmp(name, "room1_extr_stopped") == 0) extr = 0;
      else if (strcmp(name, "room1_extr_moving") == 0) extr = 1;
      else if (strcmp(name, "room1_extr_error") == 0) extr = 2;
      else if (strcmp(name, "room1_extr_extended") == 0) extr = 3;
      else if (strcmp(name, "room1_extr_retracted") == 0) extr = 4;
  
      else if (strcmp(name, "room1_shrk_stopped") == 0) shrk_platform = 0;
      else if (strcmp(name, "room1_shrk_at_left") == 0) shrk_platform = 1;
      else if (strcmp(name, "room1_shrk_at_right") == 0) shrk_platform = 2;
      else if (strcmp(name, "room1_shrk_moving") == 0) shrk_platform = 3;
      else if (strcmp(name, "room1_shrk_error") == 0) shrk_platform = 4;
  
      else if (strcmp(name, "room1_shrk_door_opened") == 0) shrk_door = 1;
      else if (strcmp(name, "room1_shrk_door_closed") == 0) shrk_door = 2;
      else if (strcmp(name, "room1_shrk_door_locked") == 0) shrk_door = 0;
        
      else if (strcmp(name, "room1_shrk_object_undetect") == 0) shrk_detect = 0;
      else if (strcmp(name, "room1_shrk_object_detect") == 0) shrk_detect = 1;
  
      else if (strcmp(name, "room1_bgame_stopped") == 0) bgame = 0;
      else if (strcmp(name, "room1_bgame_playing") == 0) bgame = 1;
      else if (strcmp(name, "room1_bgame_lose") == 0) bgame = 2;
      else if (strcmp(name, "room1_bgame_win") == 0) bgame = 3;

      else if (strcmp(name, "room1_sound_idle") == 0) room1_sound = 0;
      else if (strcmp(name, "room1_sound_busy") == 0) room1_sound = 1;
    }
    
	else if (strncmp(name, "room2", 5) == 0)
	{
  
      if (strcmp(name, "room2mockup_offline") == 0) room2_mockup = 0;
      else if (strcmp(name, "room2mockup_online") == 0) room2_mockup = 1;
      else if (strcmp(name, "room2mockup_comm_error") == 0) room2_mockup = 2;
  
      else if (strcmp(name, "room2vdoor_offline") == 0) room2_vdoor = 0;
      else if (strcmp(name, "room2vdoor_online") == 0) room2_vdoor = 1;
      else if (strcmp(name, "room2vdoor_comm_error") == 0) room2_vdoor = 2;
  
      else if (strcmp(name, "room2table_offline") == 0) room2_table = 0;
      else if (strcmp(name, "room2table_online") == 0) room2_table = 1;
      else if (strcmp(name, "room2table_comm_error") == 0) room2_table = 2;

      else if (strcmp(name, "room2_game_wait_magbridge") == 0) room2_game = 0;
      else if (strcmp(name, "room2_game_wait_keyslot1") == 0) room2_game = 1;
      else if (strcmp(name, "room2_game_wait_vinyl") == 0) room2_game = 2;
      else if (strcmp(name, "room2_game_wait_color_1") == 0) room2_game = 3;
      else if (strcmp(name, "room2_game_wait_color_2") == 0) room2_game = 4;
      else if (strcmp(name, "room2_game_wait_mockup_walls") == 0) room2_game = 5;
      else if (strcmp(name, "room2_game_wait_eyes_circuit") == 0) room2_game = 6;
      else if (strcmp(name, "room2_game_wait_ball_game") == 0) room2_game = 7;
      else if (strcmp(name, "room2_game_delay") == 0) room2_game = 8;
      else if (strcmp(name, "room2_game_ended") == 0) room2_game = 9;
      else if (strcmp(name, "room2_game_stopped") == 0) room2_game = 10;

      else if (strcmp(name, "room2_exitdoor_unlocked") == 0) room2_exitdoor = 0;
      else if (strcmp(name, "room2_exitdoor_locked") == 0) room2_exitdoor = 1;

      else if (strcmp(name, "room2_vdoor_unlocked") == 0) room2_vdoor_lock = 0;
      else if (strcmp(name, "room2_vdoor_locked") == 0) room2_vdoor_lock = 1;

      else if (strcmp(name, "room2_vinyl_disabled") == 0) room2_vinyl_enable = 0;
      else if (strcmp(name, "room2_vinyl_enabled") == 0) room2_vinyl_enable = 1;

      else if (strcmp(name, "room2_ballgame_default") == 0) room2_ballgame = 0;
      else if (strcmp(name, "room2_ballgame_finished") == 0) room2_ballgame = 1;
      
      else if (strcmp(name, "room2_stand_coil1_unlocked") == 0) room2_stand_coil1 = 0;
      else if (strcmp(name, "room2_stand_coil1_locked") == 0) room2_stand_coil1 = 1;
      else if (strcmp(name, "room2_stand_coil2_unlocked") == 0) room2_stand_coil2 = 0;
      else if (strcmp(name, "room2_stand_coil2_locked") == 0) room2_stand_coil2 = 1;
      else if (strcmp(name, "room2_stand_coil3_unlocked") == 0) room2_stand_coil3 = 0;
      else if (strcmp(name, "room2_stand_coil3_locked") == 0) room2_stand_coil3 = 1;
  
      else if (strcmp(name, "room2_eyes_circuit_undetected") == 0) room2_eyes_circuit = 0;
      else if (strcmp(name, "room2_eyes_circuit_detected") == 0) room2_eyes_circuit = 1;
  
      else if (strcmp(name, "room2_mockup_wall1_undetect") == 0) room2_mockup_wall1 = 0;
      else if (strcmp(name, "room2_mockup_wall1_detect") == 0) room2_mockup_wall1 = 1;
      else if (strcmp(name, "room2_mockup_wall2_undetect") == 0) room2_mockup_wall2 = 0;
      else if (strcmp(name, "room2_mockup_wall2_detect") == 0) room2_mockup_wall2 = 1;
      else if (strcmp(name, "room2_mockup_wall3_undetect") == 0) room2_mockup_wall3 = 0;
      else if (strcmp(name, "room2_mockup_wall3_detect") == 0) room2_mockup_wall3 = 1;
      else if (strcmp(name, "room2_mockup_wall4_undetect") == 0) room2_mockup_wall4 = 0;
      else if (strcmp(name, "room2_mockup_wall4_detect") == 0) room2_mockup_wall4 = 1;
      else if (strcmp(name, "room2_mockup_wall5_undetect") == 0) room2_mockup_wall5 = 0;
      else if (strcmp(name, "room2_mockup_wall5_detect") == 0) room2_mockup_wall5 = 1;
      else if (strcmp(name, "room2_mockup_wall6_undetect") == 0) room2_mockup_wall6 = 0;
      else if (strcmp(name, "room2_mockup_wall6_detect") == 0) room2_mockup_wall6 = 1;
  
      else if (strcmp(name, "room2_shome_voice_idle") == 0) room2_shome_voice = 0;
      else if (strcmp(name, "room2_shome_voice_busy") == 0) room2_shome_voice = 1;
  
      else if (strcmp(name, "room2_safe_hacked") == 0) room2_safe = 0;
      else if (strcmp(name, "room2_safe_safe") == 0) room2_safe = 1;
  
      else if (strcmp(name, "room2_table_magbridge_opened") == 0) room2_magbridge_circuit = 0;
      else if (strcmp(name, "room2_table_magbridge_closed") == 0) room2_magbridge_circuit = 1;

      else if (strcmp(name, "room2_table_coil1_unlocked") == 0) room2_table_coil1 = 0;
      else if (strcmp(name, "room2_table_coil1_locked") == 0) room2_table_coil1 = 1;
      else if (strcmp(name, "room2_table_coil2_unlocked") == 0) room2_table_coil2 = 0;
      else if (strcmp(name, "room2_table_coil2_locked") == 0) room2_table_coil2 = 1;
      
      else if (strcmp(name, "room2_table_led1_on") == 0) room2_table_led1 = 1;
      else if (strcmp(name, "room2_table_led1_off") == 0) room2_table_led1 = 0;
      else if (strcmp(name, "room2_table_led2_on") == 0) room2_table_led2 = 1;
      else if (strcmp(name, "room2_table_led2_off") == 0) room2_table_led2 = 0;
            
      else if (strcmp(name, "room2_keyslot1_undetected") == 0) room2_keyslot1 = 0;
      else if (strcmp(name, "room2_keyslot1_detected") == 0) room2_keyslot1 = 1;
  
      else if (strcmp(name, "room2_vdoor_sensor_light_off") == 0) room2_vdoor_sensor_light = 0;
      else if (strcmp(name, "room2_vdoor_sensor_light_on") == 0) room2_vdoor_sensor_light = 1;
  
      else if (strcmp(name, "room2_vdoor_sensor_undetected") == 0) room2_vdoor_sensor_detect = 0;

      else if (strcmp(name, "room2_vinyl_default") == 0) room2_vinyl = 0;
      else if (strcmp(name, "room2_vinyl_passed") == 0) room2_vinyl = 1;
      
      else if (strcmp(name, "room2_kid_voice_idle") == 0) room2_kid_voice = 0;
      else if (strcmp(name, "room2_kid_voice_busy") == 0) room2_kid_voice = 1;

      else if (strcmp(name, "room2_vdoor_sensor_detected") == 0) {
        room2_vdoor_sensor_detect = (short)atoi(argv[0]);
      }

      else if (strcmp(name, "room2_vdoor_sensor_data") == 0) {
        room2_vdoor_sensor_data_R = (short)atoi(argv[0]);
        room2_vdoor_sensor_data_G = (short)atoi(argv[1]);
        room2_vdoor_sensor_data_B = (short)atoi(argv[2]);
        room2_vdoor_sensor_data_W = (short)atoi(argv[3]);
      }

      else if (strcmp(name, "room1_error_count") == 0) {
        room2mockup_error_count = (short)atoi(argv[0]);
      }
      else if (strcmp(name, "room2mockup_error_count") == 0) {
        room2mockup_error_count = (short)atoi(argv[0]);
      }
      else if (strcmp(name, "room2vdoor_error_count") == 0) {
        room2vdoor_error_count = (short)atoi(argv[0]);
      }
      else if (strcmp(name, "room2table_error_count") == 0) {
        room2table_error_count = (short)atoi(argv[0]);
      }
      
      else if (strcmp(name, "room2_main_light") == 0) {
        room2_main_light = (short)atoi(argv[0]);
      }
  
      else if (strcmp(name, "room2_mockup_up_light") == 0) {
        room2_mockup_up_light = (short)atoi(argv[0]);
      }
  
      else if (strcmp(name, "room2_mockup_button_light") == 0) {
        room2_mockup_button_light = (short)atoi(argv[0]);
      }
  
      else if (strcmp(name, "room2_mockup_wall_light") == 0) {
        room2_mockup_wall_light = (short)atoi(argv[0]);
      }
  
      else if (strcmp(name, "room2_vdoor_sensor_detected") == 0) {
        room2_vdoor_sensor_detect = (short)atoi(argv[0]);
      }
      else if (strcmp(name, "room2_floorlight") == 0) {
        // room2_floorlight R G B
        room2_floorlight_R = (short)atoi(argv[0]);
        room2_floorlight_G = (short)atoi(argv[1]);
        room2_floorlight_B = (short)atoi(argv[2]);
      }
      //TODO
    }
    
    if (strcmp(name, "pong") == 0) {
      ping_get = 1;
      ping_errors = 0;
    }

    if (strcmp(name, "started") == 0) {
      quest = 1;
    }
    if (strcmp(name, "stopped") == 0) {
      quest = 0;
    }
    if (strcmp(name, "reloading") == 0) {
      quest = 2;
    }
    if (strcmp(name, "paused") == 0) {
      quest = 3;
    }

    if (strcmp(name, "elapsed") == 0) {
      elapsed = String(argv[0]);
    }
    
    if (strcmp(name, "show_vars") == 0) { 
      DEBUG_SERIAL.println(); 
      DEBUG_SERIAL.println(room2_main_light); 
      DEBUG_SERIAL.println(room2_mockup_up_light); 
      DEBUG_SERIAL.println(room2_mockup_button_light); 
      DEBUG_SERIAL.println(room2_mockup_wall_light);
      DEBUG_SERIAL.println(room2_vdoor_sensor_detect); 

      DEBUG_SERIAL.println(room2_floorlight_R);
      DEBUG_SERIAL.println(room2_floorlight_G);
      DEBUG_SERIAL.println(room2_floorlight_B);
      DEBUG_SERIAL.println(); 
    }

  
}

void parse_cpu_status()
{
	char *elems[MAX_STATUS_ELEMS];
	short num_elems;
	char *cur_elem, *next_elem;
	
	num_elems = 0;
	for(cur_elem = status_buf; cur_elem != NULL; cur_elem = next_elem)
	{
		next_elem = strchr(cur_elem, ' ');
		if(next_elem != NULL) {
			*(next_elem++) = 0;
			while(*next_elem == ' ')
				next_elem++;
		}
		
		elems[num_elems++] = cur_elem;
		if(num_elems == MAX_STATUS_ELEMS)
			break;
	}
	
	process_cpu_status(elems[0], num_elems - 1, elems + 1);
}

void read_cpu_status()
{
	if(WORK_SERIAL.available())
	{
		if(status_len == MAX_STATUS_LEN)
		{
			if(status_buf[status_len - 1] == '\r') {
				status_buf[0] = '\r';
				status_len = 1;
			} else {
				status_len = 0;
			}
			status_overflow = 1;
		}
		
		status_buf[status_len++] = WORK_SERIAL.read();
		
		if( (status_len >= 2) &&
			(status_buf[status_len - 2] == '\r') &&
			(status_buf[status_len - 1] == '\n') )
		{
			if(!status_overflow) {
				status_buf[status_len - 2] = 0;
				parse_cpu_status();
			}
			status_len = 0;
			status_overflow = 0;
		}
	}
}


void loop() {
  // обрабатываем серверных клиентов
  server.handleClient();

  unsigned long currentMillis = millis();

  read_cpu_status();
 
  // таймаут и отправка пинга
  if (currentMillis - millis_ping_send > ping_interval) {
    WORK_SERIAL.println("ping");
    ping_get = 0;
    millis_ping_send = currentMillis;
  }
  if ((ping_get == 0) && (currentMillis - millis_ping_send > ping_timeout)) {
    ping_get = 1;
    ping_errors++;
  }
  
  // светодиод активности
  if(led_activate==1&&led_start==0) {
    led_start=currentMillis;
    led_activate=0;
    digitalWrite(5, HIGH);
  }
  if ((led_start!=0)&&(currentMillis - led_start > led_duration)) {
    led_start=0;
    digitalWrite(5, LOW);
  }
}
