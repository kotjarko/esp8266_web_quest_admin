void handle_status(){
  led_activate = 1;
  if (!is_authentified()){
    server.send(200, "text/html", "");
    return;
  }

  String str;
  
  str.reserve(16384);
  
  str += F("{");

  if(ping_errors<2) str+=F("\"ping\": \"online\",");
  else str+=F("\"ping\": \"offline\",");

  str+="\"elapsed\": \""+elapsed+"\",";

  if(quest==0) str+=F("\"quest\": \"stopped\",");
  else if(quest==1) str+=F("\"quest\": \"started\",");
  else if(quest==2) str+=F("\"quest\": \"reloading\",");
  else  str+=F("\"quest\": \"paused\",");

  switch(room1_game) {
    case 0: str+=F("\"room1_game\": \"room1_game_stopped\",");
      break;
    case 1: str+=F("\"room1_game\": \"room1_game_wait_scan\",");
      break;
    case 2: str+=F("\"room1_game\": \"room1_game_wait_print_start\",");
      break;
    case 3: str+=F("\"room1_game\": \"room1_game_printing\",");
      break;
    case 4: str+=F("\"room1_game\": \"room1_game_bgame_wait\",");
      break;
    case 5: str+=F("\"room1_game\": \"room1_game_bgame_1\",");
      break;
    case 6: str+=F("\"room1_game\": \"room1_game_bgame_2\",");
      break;
    case 7: str+=F("\"room1_game\": \"room1_game_bgame_3\",");
      break;
    case 8: str+=F("\"room1_game\": \"room1_game_wait_shrk_activate\",");
      break;
    case 9: str+=F("\"room1_game\": \"room1_game_wait_shrk_object\",");
      break;
    case 10: str+=F("\"room1_game\": \"room1_game_wait_shrk_door\",");
      break;
    case 11: str+=F("\"room1_game\": \"room1_game_shrinking_object\",");
      break;
    case 12: str+=F("\"room1_game\": \"room1_game_wait_objects\",");
      break;
    default: str+=F("\"room1_game\": \"room1_game_ended\",");
      break;
  }
  
  if(room1_exit==0) str+=F("\"room1_exit\": \"room1_exit_unlocked\",");
  else str+=F("\"room1_exit\": \"room1_exit_locked\",");

  if(room1_object_1==0) str+=F("\"room1_object_1\": \"room1_no_object_1\",");
  else str+=F("\"room1_object_1\": \"room1_have_object_1\",");
  
  if(room1_object_2==0) str+=F("\"room1_object_2\": \"room1_no_object_2\",");
  else str+=F("\"room1_object_2\": \"room1_have_object_2\",");
  
  if(room1_object_3==0) str+=F("\"room1_object_3\": \"room1_no_object_3\",");
  else str+=F("\"room1_object_3\": \"room1_have_object_3\",");

  if(room1_sound==0) str+=F("\"room1_sound\": \"room1_sound_idle\",");
  else str+=F("\"room1_sound\": \"room1_sound_busy\",");
  
  if(room1_comm==0) str+=F("\"room1_online\": \"room1_offline\",");
  else if(room1_comm==1) str+=F("\"room1_online\": \"room1_online\",");
  else str+=F("\"room1_online\": \"room1_comm_error\",");

  str+="\"room1_main_light\": \""+String(main_light)+"\",";
  str+="\"room1_uv_light\": \""+String(uv_light)+"\",";
  str+="\"num_objects\": \""+String(num_objects)+"\",";

  if(scan==0) str+=F("\"room1_scan\": \"room1_scan_default\",");
  else str+=F("\"room1_scan\": \"room1_scan_passed\",");

  if(printer==0) str+=F("\"room1_printer\": \"room1_printer_stopped\",");
  else str+=F("\"room1_printer\": \"room1_printer_active\",");

  if(lift==0) str+=F("\"room1_lift\": \"room1_lift_stopped\",");
  else if(lift==1) str+=F("\"room1_lift\": \"room1_lift_moving\",");
  else if(lift==2) str+=F("\"room1_lift\": \"room1_lift_error\",");
  else if(lift==3) str+=F("\"room1_lift\": \"room1_lift_extended\",");
  else str+=F("\"room1_lift\": \"room1_lift_retracted\",");

  if(extr==0) str+=F("\"room1_extr\": \"room1_extr_stopped\",");
  else if(extr==1) str+=F("\"room1_extr\": \"room1_extr_moving\",");
  else if(extr==2) str+=F("\"room1_extr\": \"room1_extr_error\",");
  else if(extr==3) str+=F("\"room1_extr\": \"room1_extr_extended\",");
  else str+=F("\"room1_extr\": \"room1_extr_retracted\",");
  
  if(shrk_platform==0) str+=F("\"room1_shrk_platform\": \"room1_shrk_stopped\",");
  else if(shrk_platform==1) str+=F("\"room1_shrk_platform\": \"room1_shrk_at_left\",");
  else if(shrk_platform==2) str+=F("\"room1_shrk_platform\": \"room1_shrk_at_right\",");
  else if(shrk_platform==3) str+=F("\"room1_shrk_platform\": \"room1_shrk_moving\",");
  else str+=F("\"room1_shrk_platform\": \"room1_shrk_error\",");

  if(shrk_door==1) str+=F("\"room1_shrk_door\": \"room1_shrk_door_opened\",");
  else if(shrk_door==2) str+=F("\"room1_shrk_door\": \"room1_shrk_door_closed\",");
  else str+=F("\"room1_shrk_door\": \"room1_shrk_door_locked\",");

  if(shrk_detect==1) str+=F("\"room1_shrk_object\": \"room1_shrk_object_detect\",");
  else str+=F("\"room1_shrk_object\": \"room1_shrk_object_undetect\",");

  if(bgame==0) str+=F("\"room1_bgame\": \"room1_bgame_stopped\", ");
  else if(bgame==1) str+=F("\"room1_bgame\": \"room1_bgame_playing\", ");
  else if(bgame==2) str+=F("\"room1_bgame\": \"room1_bgame_lose\", ");
  else str+=F("\"room1_bgame\": \"room1_bgame_win\", ");

  if(room2_mockup==1) str+=F("\"room2_mockup\": \"room2mockup_online\",");
  else if(room2_mockup==2) str+=F("\"room2_mockup\": \"room2mockup_comm_error\",");
  else str+=F("\"room2_mockup\": \"room2mockup_offline\",");

  if(room2_vdoor==1) str+=F("\"room2_vdoor\": \"room2vdoor_online\",");
  else if(room2_vdoor==2) str+=F("\"room2_vdoor\": \"room2vdoor_comm_error\",");
  else str+=F("\"room2_vdoor\": \"room2vdoor_offline\",");

  if(room2_table==1) str+=F("\"room2_table\": \"room2table_online\",");
  else if(room2_table==2) str+=F("\"room2_table\": \"room2table_comm_error\",");
  else str+=F("\"room2_table\": \"room2table_offline\",");

  switch(room2_game) {
    case 0: str+=F("\"room2_game\": \"room2_game_wait_magbridge\","); 
      break;
    case 1: str+=F("\"room2_game\": \"room2_game_wait_keyslot1\","); 
      break;
    case 2: str+=F("\"room2_game\": \"room2_game_wait_vinyl\","); 
      break;
    case 3: str+=F("\"room2_game\": \"room2_game_wait_color_1\","); 
      break;
    case 4: str+=F("\"room2_game\": \"room2_game_wait_color_2\","); 
      break;
    case 5: str+=F("\"room2_game\": \"room2_game_wait_mockup_walls\","); 
      break;
    case 6: str+=F("\"room2_game\": \"room2_game_wait_eyes_circuit\","); 
      break;
    case 7: str+=F("\"room2_game\": \"room2_game_wait_ball_game\","); 
      break;
    case 8: str+=F("\"room2_game\": \"room2_game_delay\","); 
      break;
    case 9: str+=F("\"room2_game\": \"room2_game_ended\","); 
      break;
    default: str+=F("\"room2_game\": \"room2_game_stopped\","); 
      break;
  }
  
  if(room2_exitdoor==1) str+=F("\"room2_exitdoor\": \"room2_exitdoor_locked\",");
  else str+=F("\"room2_exitdoor\": \"room2_exitdoor_unlocked\",");

  if(room2_vdoor_lock==1) str+=F("\"room2_vdoor_lock\": \"room2_vdoor_locked\",");
  else str+=F("\"room2_vdoor_lock\": \"room2_vdoor_unlocked\",");

  if(room2_ballgame==1) str+=F("\"room2_ballgame\": \"room2_ballgame_finished\",");
  else str+=F("\"room2_ballgame\": \"room2_ballgame_default\",");

  if(room2_stand_coil1==1) str+=F("\"room2_stand_coil1\": \"room2_stand_coil1_locked\",");
  else str+=F("\"room2_stand_coil1\": \"room2_stand_coil1_unlocked\",");
  if(room2_stand_coil2==1) str+=F("\"room2_stand_coil2\": \"room2_stand_coil2_locked\",");
  else str+=F("\"room2_stand_coil2\": \"room2_stand_coil2_unlocked\",");
  if(room2_stand_coil2==1) str+=F("\"room2_stand_coil3\": \"room2_stand_coil3_locked\",");
  else str+=F("\"room2_stand_coil3\": \"room2_stand_coil3_unlocked\",");

  if(room2_eyes_circuit==1) str+=F("\"room2_eyes_circuit\": \"room2_eyes_circuit_detected\",");
  else str+=F("\"room2_eyes_circuit\": \"room2_eyes_circuit_undetected\",");

  if(room2_mockup_wall1==1) str+=F("\"room2_mockup_wall1\": \"room2_mockup_wall1_detect\",");
  else str+=F("\"room2_mockup_wall1\": \"room2_mockup_wall1_undetect\",");
  if(room2_mockup_wall2==1) str+=F("\"room2_mockup_wall2\": \"room2_mockup_wall2_detect\",");
  else str+=F("\"room2_mockup_wall2\": \"room2_mockup_wall2_undetect\",");
  if(room2_mockup_wall3==1) str+=F("\"room2_mockup_wall3\": \"room2_mockup_wall3_detect\",");
  else str+=F("\"room2_mockup_wall3\": \"room2_mockup_wall3_undetect\",");
  if(room2_mockup_wall4==1) str+=F("\"room2_mockup_wall4\": \"room2_mockup_wall4_detect\",");
  else str+=F("\"room2_mockup_wall4\": \"room2_mockup_wall4_undetect\",");
  if(room2_mockup_wall5==1) str+=F("\"room2_mockup_wall5\": \"room2_mockup_wall5_detect\",");
  else str+=F("\"room2_mockup_wall5\": \"room2_mockup_wall5_undetect\",");
  if(room2_mockup_wall6==1) str+=F("\"room2_mockup_wall6\": \"room2_mockup_wall6_detect\",");
  else str+=F("\"room2_mockup_wall6\": \"room2_mockup_wall6_undetect\",");

  if(room2_shome_voice==1) str+=F("\"room2_shome_voice\": \"room2_shome_voice_busy\",");
  else str+=F("\"room2_shome_voice\": \"room2_shome_voice_idle\",");

  if(room2_safe==1) str+=F("\"room2_safe\": \"room2_safe_safe\",");
  else str+=F("\"room2_safe\": \"room2_safe_hacked\",");

  if(room2_magbridge_circuit==1) str+=F("\"room2_magbridge_circuit\": \"room2_table_magbridge_closed\",");
  else str+=F("\"room2_magbridge_circuit\": \"room2_table_magbridge_opened\",");

  if(room2_table_coil1==1) str+=F("\"room2_table_coil1\": \"room2_table_coil1_locked\",");
  else str+=F("\"room2_table_coil1\": \"room2_table_coil1_unlocked\",");
  if(room2_table_coil2==1) str+=F("\"room2_table_coil2\": \"room2_table_coil2_locked\",");
  else str+=F("\"room2_table_coil2\": \"room2_table_coil2_unlocked\",");

  if(room2_keyslot1==1) str+=F("\"room2_keyslot1\": \"room2_keyslot1_detected\",");
  else str+=F("\"room2_keyslot1\": \"room2_keyslot1_undetected\",");

  if(room2_vdoor_sensor_light==1) str+=F("\"room2_vdoor_sensor_light\": \"room2_vdoor_sensor_light_on\",");
  else str+=F("\"room2_vdoor_sensor_light\": \"room2_vdoor_sensor_light_off\",");

  if(room2_kid_voice==1) str+=F("\"room2_kid_voice\": \"room2_kid_voice_busy\",");
  else str+=F("\"room2_kid_voice\": \"room2_kid_voice_idle\",");

  if(room2_vinyl==1) str+=F("\"room2_vinyl\": \"room2_vinyl_passed\",");
  else str+=F("\"room2_vinyl\": \"room2_vinyl_default\",");

  if(room2_vinyl_enable==0) str+=F("\"room2_vinyl_enable\": \"room2_vinyl_disabled\",");
  else str+=F("\"room2_vinyl_enable\": \"room2_vinyl_enabled\",");

  if(room2_table_led1==0) str+=F("\"room2_table_led1\": \"room2_table_led1_off\",");
  else str+=F("\"room2_table_led1\": \"room2_table_led1_on\",");

  if(room2_table_led2==0) str+=F("\"room2_table_led2\": \"room2_table_led2_off\",");
  else str+=F("\"room2_table_led2\": \"room2_table_led2_on\",");
  
  str+="\"room2_floorlight_R\": \""+String(room2_floorlight_R)+"\",";
  str+="\"room2_floorlight_G\": \""+String(room2_floorlight_G)+"\",";
  str+="\"room2_floorlight_B\": \""+String(room2_floorlight_B)+"\",";

  str+="\"room2_mockup_up_light\": \""+String(room2_mockup_up_light)+"\",";
  str+="\"room2_mockup_button_light\": \""+String(room2_mockup_button_light)+"\",";
  str+="\"room2_mockup_wall_light\": \""+String(room2_mockup_wall_light)+"\",";

  if(room2_vdoor_sensor_detect!=0) {
    str+="\"room2_vdoor_sensor_detected\": \""+String(room2_vdoor_sensor_detect)+"\",";
  }
  else str+="\"room2_vdoor_sensor_detected\": \"room2_vdoor_sensor_undetected\",";
  
    str+="\"room2_vdoor_sensor_data_R\": \""+String(room2_vdoor_sensor_data_R)+"\",";
    str+="\"room2_vdoor_sensor_data_G\": \""+String(room2_vdoor_sensor_data_G)+"\",";
    str+="\"room2_vdoor_sensor_data_B\": \""+String(room2_vdoor_sensor_data_B)+"\",";
    str+="\"room2_vdoor_sensor_data_W\": \""+String(room2_vdoor_sensor_data_W)+"\",";
  
  str+="\"room2_main_light\": \""+String(room2_main_light)+"\",";

  str+="\"room1_error_count\": \""+String(room1_error_count)+"\",";
  str+="\"room2mockup_error_count\": \""+String(room2mockup_error_count)+"\",";
  str+="\"room2vdoor_error_count\": \""+String(room2vdoor_error_count)+"\",";
  str+="\"room2table_error_count\": \""+String(room2table_error_count)+"\"";
  
  str += "}";
  
  server.send(200, "text/html", str);
}

void handle_serial_send() {

  led_activate = 1;
  // если не авторизован - редиректим (301) на логин
  if (!is_authentified()){
    handle_redirect_301();
    return;
  }
  
  // Обрабатываем комбинации значений 1\0 входящих (GET) параметров led1 и led2
  // Согласно значениям выбираем режим и отправляем в UART
  if (server.hasArg("string")) {
    //
    WORK_SERIAL.println(server.arg("string"));
    DEBUG_SERIAL.println(server.arg("string"));
  }
  if(server.hasArg("from")) {
    //String header = "HTTP/1.1 301 OK\r\nLocation: /"+server.arg("from")+"\r\nCache-Control: no-cache\r\n\r\n";
    //server.sendContent(header);    
    handle_redirect_301(server.arg("from"));
  
  }
  else {
    server.send(200, "text/html", "");
  }
}

void handle_cpu_reset() {
  led_activate = 1;
  if (!is_authentified()){
    handle_redirect_301();
    return;
  }
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delayMicroseconds(5);
  pinMode(16, INPUT);
  server.send(200, "text/html", "");
}
