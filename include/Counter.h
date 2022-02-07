#ifndef Counter_h
#define Counter_h


// lol this whole file is so unnecessary 
unsigned long counter = 42069;

void setCounter(int val){
    counter = val;
}

void incrementCounter(){
    counter++;
    // Serial.println(counter);
}

void resetCounter(){
    counter = 0;
}

#endif