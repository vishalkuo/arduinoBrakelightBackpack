int lightPin = 0;  //define a pin for Photo resistor
int threshold; 

void setup(){
    Serial.begin(9600);  //Begin serial communcation
    pinMode(13, OUTPUT);
    
    threshold = analogRead(lightPin) + 15;

}



void loop(){
    Serial.println(analogRead(lightPin)); 
    Serial.println(threshold);
    if(analogRead(lightPin) < threshold ){    
        digitalWrite(13, LOW);
        Serial.println("low"); 
    }else{
        digitalWrite(13, HIGH);
        Serial.println("high"); 
    }

    delay(100);
}
