// функция для редиректа на страницу логина
void handle_redirect_301(String page = "login") {
  led_activate = 1;
  // код 301 - браузер-сайд редирект на указанный Location
  String header = "HTTP/1.1 301 OK\r\nLocation: /"+page+"\r\nCache-Control: no-cache\r\n\r\n";
  server.sendContent(header);
}


// корневая страница со ссылками
void handle_root() {
  led_activate = 1;
  if (!is_authentified()){
    handle_redirect_301("login_form");
    return;
  }

  // выводим разный набор ссылок для первой и второй авторизации и не авторизованного
  String content = "<html><body><h1>main</h1><br>";
  if(hasSD) content+="<br>has SD<br>";
  else content+="<br>no SD<br>";
  if(!is_authentified())content += "<a href='/login'>login</a><br>";
  if(is_authentified())content += "<a href='/secure/index.htm'>admin page</a><br>";
  //if(is_root())content += "<a href='/hidden'>superadmin page</a><br>";
  if(is_authentified())content +="<a href='/login?DISCONNECT=1'>logout</a>";
  server.send(200, "text/html", content);
}

// отправляем ошибку 404 с кастомной страницой. Для старого интернет эксплорера необходима длинная кастомная страница, иначе она меняется на стандартную
void handle_404() {
  led_activate = 1;
  server.send(404, "text/html", "<h1>NOT FOUND</h1>");
}

// Страница управления, доступная обеим авторизациям
void handle_access() {
  led_activate = 1;
  // если не авторизован - редиректим (301) на логин
  if (!is_authentified()){
    handle_redirect_301();
    return;
  }

  // Вывод страницы с кнопками выбора режима. 
  // При нажатии кнопки делают переход на эту же самую страницу, но с параметрами режима светодиодов, например ?led1=0&led2=1
  String temp = "<!DOCTYPE html><html><head><title>Панель администратора квеста \"ИСКИН\"</title><meta charset='utf-8'/>";
  temp += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/></head><body><h1>Нет SD карты!</h1>";

  String str;
  if(ping_errors<2) str="установлена";
  else str="не установлена";
  temp += "<fieldset><legend>Основное управление квестом</legend>Связь с квестом: <b>"+str+"</b><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=123\"' value='Перезагрузить квест'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=full_report\"' value='Проверка устройств'><br>";
  temp += "<input type='button' onclick='location.href=\"/login?DISCONNECT=1\"' value='Выход из панели управления'><br></fieldset>";

  if(room1_comm==0) str="room1_offline";
  else if(room1_comm==1) str="room1_online";
  else str="room1_comm_error";
  temp += "<fieldset><legend>Состояние связи с управляющими модулями</legend>";
  temp += "Связь с room1: <b>"+str+"</b><br>";
  temp += "Связь с room2: <b>не установлена</b></fieldset>";

  if(printer==0) str="room1_printer_stopped";
  else str="room1_printer_active";
  temp += "<fieldset><legend>Принтер</legend> Состояние: <b>"+str+"</b><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_print_stop\"' value='stop'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_print_go_home\"' value='go_home'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_print_start\"' value='start'></fieldset>";

  if(lift==0) str="room1_lift_stopped";
  else if(lift==1) str="room1_lift_moving";
  else str="room1_lift_error";
  temp += "<fieldset><legend>Подъемное основание принтера</legend>";
  temp += "Состояние: <b>"+str+"</b><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_lift_stop\"' value='stop'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_lift_extend\"' value='extend'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_lift_retract\"' value='retract'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_lift_slow_extend\"' value='slow extend'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_lift_slow_retract\"' value='slow retract'></fieldset>";  

  if(extr==0) str="room1_extr_stopped";
  else if(extr==1) str="room1_extr_moving";
  else str="room1_extr_error";
  temp += "<fieldset><legend>\"Выбрасыватель\" фигур</legend>";
  temp += "Состояние: <b>"+str+"</b><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_extr_stop\"' value='stop'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_extr_extend\"' value='extend'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_extr_retract\"' value='retract'></fieldset>";

  if(shrk_platform==0) str="room1_shrk_stopped";
  else if(shrk_platform==1) str="room1_shrk_moving";
  else str="room1_shrk_error";
  temp += "<fieldset><legend>Уменьшитель</legend>";
  temp += "Состояние: <b>"+str+"</b><br>";
  if(shrk_detect==1) str="room1_shrk_detect";
  else str="room1_shrk_undetect";     
  temp += "Состояние: <b>"+str+"</b><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_shrk_stop\"' value='stop'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_shrk_left\"' value='left'><br>";
  temp += "<input type='button' onclick='location.href=\"/serial?from=access&string=room1_shrk_right\"' value='right'>";
  temp += "</fieldset></body></html>";
  server.send ( 200, "text/html", temp );
}


void handle_login_form() {
  led_activate = 1;
  // Вывод формы авторизации, куков для отладки и сообщения об ошибке, если есть
  String content = "<html><body><form action='/login' method='POST'>admin/admin, root/root<br>";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form><br>";
  server.send(200, "text/html", content);
}

// функция обработки страницы логина, включает в себя обработку логина-пароля, установку правильных куков, а также сброс авторизации ("выход")
void handle_login() {
  // led_activate = 1;
  String msg = "";
  String cookie = "";
  // для отладки, кладем все куки в переменную
  if (server.hasHeader("Cookie")){   
    cookie = server.header("Cookie");
  }
  // "выход"
  if (server.hasArg("DISCONNECT")){
    // устанавливаем обе куки в ноль и отправляем на редирект на страницу логина (301)
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: "+session_name+"=0\r\nSet-Cookie: root=0\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  // попытки авторизации, если есть входящие значения из формы
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
    // первая авторизация, если имя и пароль соответствуют первой паре значений. 
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin" ){
      // отправляем на редирект (301) на главную страницу и устанавливаем куку первой авторизации
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: "+session_name+"="+generate_check_sessionid()+"\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);    
      return;
    }
    // вторая авторизация, если имя и пароль соответствуют второй паре значений. 
    else if (server.arg("USERNAME") == "root" &&  server.arg("PASSWORD") == "root" ){
      // отправляем на редирект (301) на главную страницу и устанавливаем куку второй авторизации
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: "+session_name+"="+generate_check_sessionid()+"\r\nSet-Cookie: root=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header); 
      return;      
    }
    // если имя и\или пароль не соответсвуют обеим парам значений
    else {
      if(hasSD) handle_redirect_301("403.htm");
      else handle_redirect_301("login_form");
      return;
    }
  }
  String content = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/></head><body><h1>Неизвестная ошибка, обратитесь к администратору</h1></body></html>";
  server.send(200, "text/html", content);  
}
