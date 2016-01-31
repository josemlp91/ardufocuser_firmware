// DS1302_Serial_Easy (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics


// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4

// screen /dev/ttyUSB0 9600

// DS1302_Serial_Easy (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics


// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4

#include <DS1302.h>
#include <Shell.h>

// Init the DS1302
DS1302 rtc(4, 3, 2);
Time t;

void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  Serial.begin(9600);
  shell_init(shell_reader, shell_writer, 0);
  shell_register(set_time, "SETIME");
  shell_register(current_time, "CURRENTIME");

}

void loop()
{
  shell_task();
}

// Comando para cambiar la hora.
int set_time(int argc, char** argv)
{

    if (argc > 3){
      shell_printf("Cambiando hora a: HORA: \"%s\"   - MINUTOS: \"%s\"  - SEG: \"%s\" \r\n", argv[1], argv[2],argv[3]);

      String str_hour=argv[1];
      String str_min=argv[2];
      String str_seg=argv[3];

      int new_hour=str_hour.toInt();
      int new_min=str_min.toInt();
      int new_seg=str_seg.toInt();

      rtc.setTime(new_hour,new_min,new_seg);     // Set the time to 12:00:00 (24hr format)

    }else{
      shell_printf("SETTIME <H> <M> <S>");
    }

  return SHELL_RET_SUCCESS;
}

// Comando para cambiar la hora.
int current_time(int argc, char** argv)
{
  shell_printf(rtc.getTimeStr());
  return SHELL_RET_SUCCESS;
}


int shell_reader(char * data)
{
  if (Serial.available()) {
    *data = Serial.read();
    return 1;
  }
  return 0;
}


void shell_writer(char data)
{
  Serial.write(data);
}
