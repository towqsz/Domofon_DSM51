#include <at89x51.h>
#include <absacc.h>

#define LCDWC XBYTE[0xff80] //LCD write control
#define LCDWD XBYTE[0xff81] //LCD write data
#define LCDRC XBYTE[0xff82] //LCD read control (status)
#define LCDRD XBYTE[0xff83] //LCD read data

#define CSKB0 XBYTE[0xff21] //stan kb1 (0-7)
#define CSKB1 XBYTE[0xff22] //stan kb2 (pozostale)

void wait(unsigned int d) {
	unsigned int i,j;
	for (j=0; j<d; j++)
    	for (i=0; i<144; i++);
}


void write_control(unsigned char a){	//zapis rozakazu, zapis danej
	while ( XBYTE[0xff82] & 0x80 ) wait(1);
	XBYTE[0xff80]=a;	
}


void write_data(unsigned char a){
	while ( XBYTE[0xff82] & 0x80 ) wait(1);
	XBYTE[0xff81]=a;	
}


void lcd_clr(void){
	write_control(0x01);
	wait(10);
}

//inicjalizacja wyswietlacza
void lcd_init (void) {
unsigned char i;
	static const unsigned char code init_tab[] ={0x33, 0x32, 0x38, 0x08, 0x0f, 0x06};
	for (i = 0; i < sizeof init_tab; ++i) {
        write_control(init_tab[i]); 
        wait(1);
	}
	lcd_clr();
	write_control(0x80);            // wybierz adres DD RAM
	wait(1);
}

//program obslugi wswietlacza
void lcd(unsigned char c) {
    unsigned char x;
        switch(c){				//znakow
case'\n':						//od nowego wiersza
		x=XBYTE[0xFF82];
		if (x<=0x0f) write_control(0xc0)	;
		if((x>=0x41)&&(x<=0x50)){
		write_control(0x80);
		}
		break;
case'\r':
		XBYTE[0xFF82];
		if((x>=0x00)&(x<=0x0f))write_control(0x80);	//rozkaz dana
		if((x>=0x41)&(x<=0x50))write_control(0xc0);	//
		break;
default:
        if(c>0x1f && c,0x80) write_data(c);
    	wait(1);
        break;
	}
wait(1);
XBYTE[0xFF82];
if(x==0x10)write_control(0xc0);			//rozkaz dana
if(x==0x50)write_control(0x80);			//
}

int read(unsigned char tab[], int i)
{
	
	if(~XBYTE[0xF021]==0x1)	 //0
		{tab[i]='0'; return 1;}
	else if(~XBYTE[0xF021]==0x02)  //1
		{tab[i]='1'; return 1;}
	else if(~XBYTE[0xF021]==0x04)  //2
		{tab[i]='2'; return 1;}
	else if(~XBYTE[0xF021]==0x08)  //3
		{tab[i]='3'; return 1;}
	else if(~XBYTE[0xF021]==0x10)  //4
		{tab[i]='4'; return 1;}
	else if(~XBYTE[0xF021]==0x20)  //5
		{tab[i]='5'; return 1;}
	else if(~XBYTE[0xF021]==0x40)  //6
		{tab[i]='6'; return 1;}
	else if(~XBYTE[0xF021]==0x80)  //7
		{tab[i]='7'; return 1;}
	else if(~XBYTE[0xF022]==0x01)   //8
		{tab[i]='8'; return 1;}
	else if(~XBYTE[0xF022]==0x02)   //9
		{tab[i]='9'; return 1;}

	  return 0;
}

void set_light(int l)
{
	if(l==1)
	{
		P1_6 = 1;
		XBYTE[0xF030] = 0x40;
		XBYTE[0xF038] = 0x10;
		P1_6 = 0;
	}
	else if (l==0)
	{
		P1_6 = 1;
		XBYTE[0xF030] = 0;
		XBYTE[0xF038] = 0;
		P1_6 = 0;
	}

	else
	{
	 	P1_6 = 1;
		XBYTE[0xF030] = 0x40;
		XBYTE[0xF038] = 0x20;
		P1_6 = 0;
	}
}

void write_lcd(unsigned char tab[])
{
	int i;

	for (i=0 ; tab[i] ; i++)
		lcd(tab[i]);

	

}

void lcd_line(int l)
{
	if(l==1)
		lcd('\r');
	else
	{
		lcd('\r');
		lcd('\n');
	}
}



int delay(int d)
{  
	unsigned int i,j, tab[1];
	for (j=0; j<d; j++)
    	for (i=0; i<144; i++)
				if(read(tab,0))
				{
					wait(10);
					while(read(tab,0));
					return 1;
				} else if (~XBYTE[0xF022]==0x04) {
					set_light(1);
					wait(2000);
					set_light(0);
					return 1;
					}
	return 0;
}
 
int check_password(unsigned char tab[], unsigned char password[])
{
	int i;

	for (i=0 ; i<4 ; i++)
		if (tab[i]!=password[i])
			return 0;

	return 1;
}

void clear_line(int line)
{
	int i;
	lcd_line(line);
	for (i=0 ; i< 15 ; i++)
		lcd(' ');
	lcd_line(line);

}

void read_tab(unsigned char tab[], int l)
{
	int i;
	for(i=0 ; i<l ; )
		{
			if(read(tab,i))
			{
				wait(20);
				while(read(tab,i));
				i++;
				lcd('*');

				
			}
		}
}

void copy_tab(unsigned char tab1[], unsigned char tab2[])
{
	int i;
	for(i = 0 ; tab2[i] ; i++)
	{
		tab1[i]=tab2[i];
	}
	tab1[i]='\0';

}


int admin(unsigned char tab[], unsigned char postcode[])
{
	static unsigned char password[4]="0000";
	int i;

	if(check_password(tab, password))
	{
		P1_7=0;
		lcd_line(2);
		clear_line(2);
		read_tab(tab, 4);
		if (tab[0]=='2' && tab[1]=='2' && tab[2]=='1' && tab[3]=='1')
		{
			clear_line(2);
			read_tab(tab,4);
			P1_7=1;
			if(!check_password(tab, password))
			{
				copy_tab(postcode, tab);
				set_light(1);
				delay(100);
				set_light(0);
				return 1;
			} else
			{
				set_light(2);
				delay(100);
				set_light(0);
			}
		}
		else if	(tab[0]=='3' && tab[1]=='3' && tab[2]=='8' && tab[3]=='8')
		{
		 	clear_line(2);
			read_tab(tab,4);
			P1_7=1;
			if(!check_password(tab, postcode))
			{
				set_light(1);
				delay(100);
				set_light(0);
				copy_tab(password, tab);
				return 1;
			} else
			{
				set_light(2);
				delay(100);
				set_light(0);
			}
		}
		else
			P1_7=1;
			return 0;

	}

}



int postman(unsigned char tab[])
{
	static unsigned char postcode[4]="1234";
	if(check_password(tab, postcode))
		{
		 set_light(1);
		 delay(50);
		 set_light(0);
		 return 1;
		}

	else if (admin(tab, postcode))
		return 1;

	return 0;
	
}

void check_string(int l)
{	
	int i;
	unsigned char strings[3][15] = {"Wybierz numer:", "Dzwonie...", "Bledny numer.."}; 
	lcd_clr();
	
	
	if(l==2)
	{
		for(i=0 ; strings[l][i] ; i++)
		{
			if(i==15)
				lcd('\n');
			lcd(strings[l][i]);	
		}
		delay(100);
		wait(20);
		check_string(0);		

		
	}
	else
	 	for	(i=0 ; strings[l][i] ; i++)
			lcd(strings[l][i]);

	if (l==1)
	{
		for(i=0 ; i<1000 ; i++)
		{
			P1_5=0;
			if(delay(100))
				{
					P1_5=1;
					break;
				}
			P1_5=1;
			if(delay(50))
				break;
		}
		wait(20);
		check_string(0);
	}
	
	

}



int check_number(unsigned char tab[], counter)
{
	  	unsigned char upper_limit[2]={'5', '4'};					   //numery mieszkan
		unsigned char	lower_limit= '1';

		if(counter>2)
		{
			if(postman(tab))
				check_string(0);
			else 
				check_string(2);	
		}
		else
		{
		  	if((tab[0]<upper_limit[0] || (tab[0]==upper_limit[0] && tab[1] <= upper_limit[1]) )&& tab[0]>=lower_limit)
		  	{
		  		check_string(1);
		  	}
			else 
				check_string(2);
		}
}

void check_keyboard(unsigned char tab[])
{
			int i, counter=0;

			if(read(tab, counter))
			{
				lcd_line(2);
				for (i=0 ; i<4 ; i++)
					lcd(' ');
				lcd_line(2);
				lcd(tab[counter]);
				while(read(tab, counter));
				counter++;

				for (i=0 ; i <5000 ; i++)
					if(read(tab, counter))
					{
						lcd(tab[counter]);
						counter++;
						i=0;

						while(read(tab, counter));
						if(counter>=4)
							break;
							
					}
				tab[counter]='\0';
				check_number(tab, counter);
									

			}

}

int main(){


		unsigned char tab[5];
		int i;
		lcd_init();
		check_string(0);	    

		while(1){
			
			check_keyboard(tab);

		}
	   		
}