

bool loadFromSdCard(String path) {
  if((path.indexOf("secure")!=-1)&&(!is_authentified())) {
    handle_redirect_301();
    return true;
  }
  
  if (path.endsWith("/")) path += "index.htm";
  
  String dataType = "application/octet-stream";
  if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";  
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  else if (path.endsWith(".txt")) dataType = "text/plain";

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile) return false;

  //server.streamFile(dataFile, dataType);
  //////////////////////////////////
  server.sendHeader("Content-Length", String(dataFile.size()));
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "max-age=360000");
  server.send(200, dataType.c_str(), "");

  WiFiClient client = server.client();
  size_t totalSize = dataFile.size();
  if (client.write(dataFile, HTTP_DOWNLOAD_UNIT_SIZE) != totalSize) {
    //DBG_OUTPUT_PORT.println("Sent less data than expected!");
  }
  
  //server.streamFile(dataFile, dataType);
  //////////////////////////////////
  
  dataFile.close();
  return true;
}

void handle_404_SD() {
  if (hasSD && loadFromSdCard(server.uri())) return;
  
  
  String header = "HTTP/1.1 301 OK\r\nLocation: /404.htm\r\nCache-Control: no-cache\r\n\r\n";
  server.sendContent(header); 
}

// корневая страница со ссылками
void handle_root_SD() {
  if(!is_authentified()) {
    handle_redirect_301("index.htm");
    return;
  }
  else {
    handle_redirect_301("secure/index.htm");    
  }
}
