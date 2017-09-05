void setup(){
  Serial.begin(9600);
  Serial.println("Comienza...");
}

void loop(){
  Serial.println(freeRam());
  
  delay(30000);
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
