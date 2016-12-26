

// в будущем - функция для генерации и проверки идентификатора сессии
String generate_check_sessionid() {
  // TODO
  return "1";
}

// функция проверки аутентификации
bool is_authentified() {
  // если в принятом заголовке есть куки
  if (server.hasHeader("Cookie")){   
    String cookie = server.header("Cookie");
    // если кука содержит правильные идентификатор и знанчение куки
    if (cookie.indexOf(session_name+"="+generate_check_sessionid()) != -1) {
      // авторизован
      return true;
    }
  }
  // не авторизован
  return false;  
}

// проверка "второй авторизации"
bool is_root() {
  // если в принятом заголовке есть куки
  if (server.hasHeader("Cookie")){   
    String cookie = server.header("Cookie");
    // если кука содержит правильные идентификатор и знанчение куки
    if (cookie.indexOf("root=1") != -1) {
      // авторизован
      return true;
    }
  }
  // не авторизован
  return false;  
}
