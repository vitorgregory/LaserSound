#define ATMEGA32U4

#include <MIDIUSB.h> d
#include <MIDI.h>
#include <SoftwareSerial.h>

int current_key = 1;

MIDI_CREATE_DEFAULT_INSTANCE();

// BOTOES
const int N_BUTTONS = 11; //*  número total de botões
const int BUTTON_ARDUINO_PIN[N_BUTTONS] = {2,3,4,5,6,7,8,9,10,11}; //* pinos de cada botão

int buttonCState[N_BUTTONS] = {1};        // armazena o valor atual do botão
int buttonPState[N_BUTTONS] = {0};        // armazena o valor anterior do botão

// debounce
unsigned long lastDebounceTime[N_BUTTONS] = {0};  // a última vez que o pino de saída foi alternado
unsigned long debounceDelay = 5;    //* o tempo de debounce; aumentar se a saída estiver mandando muitas notas de uma vez so

byte midiCh = 1; //* Canal MIDI a ser usado
byte note = 48; //* nota mais baixa a ser usada
byte cc = 1; //* O mais baixo MIDI CC a ser usado




void setup() {

  Serial.begin(115200);

  

  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
  }

}

void loop() {

  buttons();
  buttons_bt();
}


void buttons_bt(){

   
   for (int i = 0; i < current_key; i++) {
  
        if (Serial.available() > 0) {
    char current_key = Serial.read();
    Serial.println (current_key);
     
     if (current_key == 'A') {
      byte note = 36;
      
     }

     else if (current_key =='B') {
      byte note = 62;
 
     }
     else if (current_key =='C') {
      byte note = 38;

     }
     else if (current_key =='D') {
      byte note = 39;

     }
  }
}
}
  
void buttons(){

      for (int i = 0; i < N_BUTTONS; i++) {

    buttonCState[i] = digitalRead(BUTTON_ARDUINO_PIN[i]);

      if ((millis() - lastDebounceTime[i]) > debounceDelay) {

        if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

          if (buttonCState[i] == LOW) {
   
          noteOn(midiCh, note + i, 127);  // channel, note, velocity
          MidiUSB.flush();

        }
        else {
          noteOff(midiCh, note + i, 0);  // channel, note, velocity
          MidiUSB.flush();

        }
        buttonPState[i] = buttonCState[i];
   }
  }  
 }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
