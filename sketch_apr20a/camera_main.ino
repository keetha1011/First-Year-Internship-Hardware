void* thread_security_camera( void* arg ) {
  while(true) {
    if ( digitalRead(PIR_PIN) == HIGH ) {
      now = time(nullptr);
      timeinfo = localtime(&now);
      //FILE_PHOTO_PATH[20];
      strftime(FILE_PHOTO_PATH, sizeof(FILE_PHOTO_PATH), "/%Y%m%d%H%M%S.jpg", timeinfo);
      
      capturePhotoSaveLittleFS(FILE_PHOTO_PATH);
      takeNewPhoto = false;
    }

    delay(5);
    
    if (Firebase.ready() && !takeNewPhoto){
      takeNewPhoto = true;
      Serial.print("Uploading picture... ");
      //BUCKET_PHOTO[50];
      strcpy(BUCKET_PHOTO, "/");
      strcat(BUCKET_PHOTO, uid.c_str());
      strcat(BUCKET_PHOTO, "/");
      strcat(BUCKET_PHOTO, FILE_PHOTO_PATH);

      if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID , FILE_PHOTO_PATH , mem_storage_type_flash , BUCKET_PHOTO , "image/jpeg" ,fcsUploadCallback)){
        Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
      }
      else{
        Serial.println(fbdo.errorReason());
      }
    }

    delay(15);
  }
}

