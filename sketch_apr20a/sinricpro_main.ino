void* thread_SinricPro( void* arg ) {
  while(true) {
    SinricPro.handle();

    delay(10);
  }
}