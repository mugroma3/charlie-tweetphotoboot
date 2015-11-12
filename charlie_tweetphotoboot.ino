#include <Adafruit_NeoPixel.h>

//led strip pin
const int stripPort = 6;

// key that will be pressed by arduino
const char keyTrigger = 'i';

// buzzer pin
const int buzzerPin = 12;

//sonar
const int triggerPort = 7;
const int echoPort = 8;
const int presence_threshold = 2;
const int min_dist=5; const int max_dist=70;

//face persistence: max_sonar_rate * presence_threshold = 1second
const int max_keyboard_rate=3000;
const int max_sonar_rate=200;

int presence_counter=0;

void toggleFaceRead(){
	//trigger sonar: high pulse for 10 microseconds
	digitalWrite(triggerPort, LOW);
	digitalWrite(triggerPort, HIGH);
	delayMicroseconds(10);
	digitalWrite(triggerPort, LOW);

	//140 cm max
	long duration = pulseIn(echoPort, HIGH, 8192);
	long r = 0.01717 * duration;

	if(r < max_dist && r>min_dist){
		presence_counter++;
	}else{
		presence_counter=0;
	}

}

bool thereIsFace(){
        return presence_counter >= presence_threshold;
}

void resetFaceEvent(){
	presence_counter=0;
}


//buzzer
const int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
// mid C C# D D# E F F# G G# A

void buzz(){
    for (int i = 0; i < numTones; i++){
        tone(speakerPin, tones[i]);
        delay(63);
    }
    noTone(speakerPin);
}
    

//led strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, stripPort);

void fill(uint8_t r, uint32_t g, uint8_t b){
	for(int i=0; i<strip.numPixels(); i++){
		strip.setPixelColor(i, r, g, b);
	}
}

void theaterChase(uint32_t c, uint8_t wait) {
	for (int j=0; j<10; j++) {  //do 10 cycles of chasing
		for (int q=0; q < 3; q++) {
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, c);    //turn every third pixel on
			}
			strip.show();
			delay(wait);
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, 0);        //turn every third pixel off
			}
		}
	}
}



unsigned long keyboard_activation_lastTime; 
unsigned long sonar_activation_lastTime; 

void setup() {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'

	pinMode(triggerPort, OUTPUT);
	pinMode(echoPort, INPUT);

	pinMode(13, OUTPUT); //led

	pinMode(buzzerPin, OUTPUT);
	
	delay(5000);
        
	Keyboard.begin();
	keyboard_activation_lastTime=millis();
	sonar_activation_lastTime=millis();
}

void loop(){
	if((millis() - sonar_activation_lastTime) > max_sonar_rate){
		sonar_activation_lastTime=millis();
		toggleFaceRead();
	}

	bool hasToFlash=false;
	if(thereIsFace() && (millis() - keyboard_activation_lastTime) >max_keyboard_rate){
		digitalWrite(13, HIGH);
		resetFaceEvent();
		keyboard_activation_lastTime=millis();
		hasToFlash=true;
		Keyboard.write(keyTrigger);
		digitalWrite(13, LOW);
	}

	if(hasToFlash){
		//it is ok to block
		fill(255, 128, 128);
		strip.show();
		delay(500);
		theaterChase(strip.Color(127, 127, 127), 50); // White

		buzz();
		fill(0, 0, 0);
		strip.show();
		
	}
}

