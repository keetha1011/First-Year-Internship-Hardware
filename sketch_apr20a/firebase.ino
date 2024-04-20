void fcsUploadCallback(FCS_UploadStatusInfo info){
    if (info.status == firebase_fcs_upload_status_init){
        Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
    }
    else if (info.status == firebase_fcs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    }
    else if (info.status == firebase_fcs_upload_status_complete)
    {
        print_image_details();
    }
    else if (info.status == firebase_fcs_upload_status_error){
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
} 

void print_image_details() {
  Serial.println("Upload completed\n");
  FileMetaInfo meta = fbdo.metaData();
  Serial.printf("Name: %s\n", meta.name.c_str());
  Serial.printf("Bucket: %s\n", meta.bucket.c_str());
  Serial.printf("contentType: %s\n", meta.contentType.c_str());
  Serial.printf("Size: %d\n", meta.size);
  Serial.printf("Generation: %lu\n", meta.generation);
  Serial.printf("Metageneration: %lu\n", meta.metageneration);
  Serial.printf("ETag: %s\n", meta.etag.c_str());
  Serial.printf("CRC32: %s\n", meta.crc32.c_str());
  Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
  Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
}