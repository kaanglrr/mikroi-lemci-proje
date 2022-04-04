#include <msp430g2553.h>

#define RED	  BIT1
#define GREEN	  BIT6
#define BLUE	  BIT5
void pwmayar();
void pwm_kirmizi(int value);
void pwm_yesil(int value);
void pwm_mavi(int value);
void setrgb(double rv,double gv,double bv);
void sicaklikrenksec();
unsigned long sicaklik = 0;
unsigned long adcdeger = 0; 
unsigned long sicaklik1 = 0;
void adc();
double rv1 = 0;
double gv1 = 0;
double bv1 = 0;
double rv2 = 0;
double gv2 = 0;
double bv2 = 0;
double rv3 = 0;
double gv3 = 0;
double bv3 = 0;
int d = 0;


void sicaklikrenksec(){
  if (sicaklik < 20){                   // Burada ADC modülünden okunup derece cinsine çevrilen sicaklik degerine uygun if kosulunu saglayan kisim aktif olur 
    setrgb(0,0,255);                    // ve o if'in içindeki setrgb fonksiyonu çalisir.
  }
  if (sicaklik >=20 && sicaklik < 22){
    setrgb(0,80,255);
  }
  if (sicaklik >= 22 && sicaklik < 24){
    setrgb(0,180,255);
  }
  if (sicaklik >= 24 && sicaklik < 26){
    setrgb(0,255,255);
  }
  if (sicaklik >= 26 && sicaklik < 28){
    setrgb(0,255,180);
  }
  if (sicaklik >= 28 && sicaklik < 30){
    setrgb(0,255,80);
  }
  if (sicaklik >= 30 && sicaklik < 32){
    setrgb(0,255,0);
  }
  if (sicaklik >= 32 && sicaklik < 34){
    setrgb(80,255,0);
  }
  if (sicaklik >= 34 && sicaklik < 36){
    setrgb(180,255,0);
  }
  if (sicaklik >= 36 && sicaklik < 38){
    setrgb(255,255,0);
  }
  if (sicaklik >= 38 && sicaklik < 40){
    setrgb(255,180,0);
  }
  if (sicaklik > 40 && sicaklik < 42){
    setrgb(255,80,0);
  }
  if (sicaklik >= 42){
    setrgb(255,0,0);
  }
}
void main()
{
WDTCTL = WDTPW + WDTHOLD;     //watchdog time kapatildi
BCSCTL1 = CALBC1_1MHZ;        // islemcinin çalisma frekansi olarak 1MHz seçildi.
DCOCTL  = CALDCO_1MHZ;

P2DIR |= (RED | BLUE);          // En basta RED (2.1), BLUE (2.5) olarak tanimladigim BIT1 ve BIT5 degerlerini P2DIR ile çikis olarak tanimliyorum ve pwm sinyali için de P2SEL ile tanimliyorum.
P2SEL |= (RED | BLUE);

P1DIR |= GREEN;                 // Ayni sekilde burada da önceden GREEN (1.6) olarak tanimladigim BIT6 degerini P1DIR ile çikis olarak tanimliyorum ve pwm sinyali için de P1SEL ile tanimliyorum. 
P1SEL |= GREEN;
adc();                          // ADC modülünün ayarlarinin oldugu fonksiyonu çagirdim.
pwmayar();                      // PWM sinyali üretilecek timerin ayarlarinin oldugu fonksiyonu çagirdim.

__bis_SR_register(GIE);         // interruptlari aktif ettim.
while(1){                       // while(1) ile fonksiyon sonsuz döngüye alindi yani sürekli sicaklik degerleri ve renkler güncellenecek.
  ADC10CTL0 |= ENC + ADC10SC;       // ENC biti ile ADC tetiklendi.
  sicaklikrenksec();                // Sicaklik degerleri kullanilarak rgb renklerin üretilecegi fonksiyon çagirildi.
   
}
}
void adc() {
  ADC10CTL0 &= ~ENC;                            // ADC ayarlari yapilmaya baslanmadan ENC biti 0 landi.
  ADC10CTL1 = INCH_5 + ADC10SSEL_3 + ADC10DIV_4;  //INCH_5 komutu ile adc için 5. kanali seçtim, ADC10SSEL_3 ile clock olarak SMCLK seçtim(çalisma frekansi 1MHz), ADC10DIV_4 ile de adc'nin deger olusturma frekansi olarak çalisma frekansinin 4 te biri yani 250KHz ayarladim.
  ADC10CTL0 = ADC10SHT_1 + ADC10ON + ADC10IE;    //ADC10ON ile adc aktif hale getirdim, ADC10IE ile interruptlari aktif hale getirdim, ADC10SHT_1 ile de adc örneginin kaç adc clock darbesinde alinacagi modunu 01 modu olan 8 x ADC10CLKs olarak ayarladim.
  ADC10AE0  = BIT5;                              // ADC girisi olarak BIT5(1.5) pinini belirledim.
   }
#pragma vector = ADC10_VECTOR     // ADC interrupt fonksiyonu cagirildi
__interrupt void ADC10_ISR (void){       
    adcdeger = ADC10MEM;             //ADC10MEM registerindan okunan degerleri adcdeger ismi verdigim degiskene aktardim.
    sicaklik1 = ((adcdeger*3300)/1024);  // okudugumuz degerler 0-1024 arasindadir bu yüzden bu degerleri mV cinsine çevirdim.
    sicaklik = sicaklik1/10;            //LM35DZ sicaklik sensöründe her 10 mV degerinde 1 derece desisim olur bu yüzden mV' a çevirdigim degeri 10 a böldüm.
   }
void setrgb(double rv,double gv,double bv){
  rv2 = (rv - rv1)/ 30;         //burada animasyon olarak, yeni bir renk degeri atandigi zaman önceki renk degeriyle arasindaki tonlar görülebilecek sekilde
  gv2 = (gv - gv1)/ 30;         //kademeli olarak geçilmesini kullandim. Bu söyle çalisiyor örnegin kirmizi için yeni gelen rv degerinin önceki rv degeriyle
  bv2 = (bv - bv1)/ 30;         //yani rv1 degerile farki aliniyor, sonrasinda bu fark 30 a bölünüyor ve bu deger rv2 ye kaydediliyor.
  
  while(d < 30){
    d++;                        //burada ise aradaki farki 30 a böldügümüz için 30 defa çalisacak bir while fonksiyonu olusturuluyor ve önceki renk degeri(rv1) her bir 
    rv1 = rv1 + rv2;            //döngüde farkin 30 a bölümüyle(rv2) toplanip rv1 e aktariliyor bu sekilde en basta yollanan rv degeri ile rv1 in arasindaki 30 degerin de 
    gv1 = gv1 + gv2;            //renk degeri olusturularak rgb ledde geçis animasyonu olusturulmus oluyor.
    bv1 = bv1 + bv2;            
    rv3 = ((rv1/255)*1000);     //rgb renk tonlari 0-255 arasinda tanimlanior fakat bizim olusturdugumuz pwm sinyali için periyodumuz 1000 bu yüzden 0-255 arasindaki degeri
    gv3 = ((gv1/255)*1000);     // 255 e bölüp 1000 ile çarptigimizda o renk için gerekli olan pwm sinyalinin degerini bulmus oluruz.
    bv3 = ((bv1/255)*1000);     
    pwm_kirmizi((int)rv3);      //bulmus oldugumuz pwm sinyal degerlerini tamsayiya çevirip, renkleri olusturan pwm fonksiyonlarina yollariz.
    pwm_yesil((int)gv3);        
    pwm_mavi((int)bv3);
  __delay_cycles(100000);
  }
  d = 0;
}
void pwmayar(){
TA1CCR0 = 1000;                 //PWM sinyallerini üretmek için timer kullaniriz. TA1CCR0 ve TA0CCR0 registerlarimiza atadigimiz degerler ile PWM sinyalinin periyodunu,
TA1CCTL1 = OUTMOD_7;            //TA1CCR1,TA1CCR2 ve TA0CCR1 registerlarina atadigimiz degerler ile de PWM sinyalinin darbe genisligini(pulse width) belirleriz. 
TA1CCTL2 = OUTMOD_7;            //TA1CCTL1,TA1CCTL2 ve TA0CCTL1 registerlarini OUTMOD_7(reset/set) ye ayarladim, bu mod timerin TACCR1 degerine ulasana kadar output=set olmasini,
TA1CTL = TASSEL_2 | MC_1;       //TACCR1 ve TACCR0 arasinda output=reset olmasini saglar ve bu reset ve set oranlarini kullanarak ledlerin parlakliklarini ayarlariz. 
                                //TASSEL_2 ile clock olarak SMCLK (1MHz) seçilir , MC_1 ile de up mode(0 dan TACCR0 a kadar sayma) aktif edilir.
TA0CCR0 = 1000;                 //TA0CCR0 ve TA1CCR0 registerlarina 1000 degeri vererek PWM sinyalinin frekansini 1KHz olarak ayarladim.
TA0CCTL1 = OUTMOD_7;

TA0CTL |= TASSEL_2 | MC_1;
}
void pwm_kirmizi(int value){    //bu 3 fonksiyonla pwm sinyalinin periyot oranlarini belirleyen degerleri belirleyerek rgb renklerimizi elde ederiz.
TA1CCR1 = value;        //kirmizi led (2.1)
}
void pwm_yesil(int value){
TA0CCR1 = value;        //yesil led (1.6)
}
void pwm_mavi(int value){
TA1CCR2 = value;        //mavi led (2.5)
}
