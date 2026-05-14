import processing.serial.*;
Serial port;
int reception;
int sd1=0;
int sd2=0;
int sd3=0;
int sd4=0;
int sd5=0;

void setup()
{
  size(600, 600);
  port = new Serial(this, "COM6", 9600);
  noStroke();
  background(0);
  strokeWeight(8);
  stroke(0, 0, 255);
  
  line(50, 50, 250, 50);
  line(145  ,40, 145, 60);
  line(50, 150, 250, 150);
  line(145  ,140, 145, 160);
  line(50, 250, 250, 250);
  line(145  ,240, 145, 260);
  line(50, 350, 250, 350);
  line(145  ,340, 145, 360);
  line(50, 450, 250, 450);
  line(145  ,440, 145, 460);
  
  stroke(255, 0, 255);
line(145  ,25, 145, 75);
  line(145, 125, 145, 175);
  line(145, 225, 145, 275);
  line(145, 325, 145, 375);
  line(145, 425, 145, 475);
  fill(255);
  textSize(50);
  text(sd1, 350, 70);
  text(sd2, 350, 170);
  text(sd3, 350, 270);
  text(sd4, 350, 370);
  text(sd5, 350, 470);
}

void draw()
{
  noStroke();
  fill(0);
  rect(0, 0, 600, 600);
  stroke(255, 0, 255);
  line(mouseX, 0, pmouseX, 600);
  line(0, mouseY, 600, pmouseY);

}

//void mouseDragged()
void mousePressed()
{
  if (mouseX>=50 && mouseX<=250  && mouseY>=25 && mouseY<=75)
  {
    noStroke();
    fill(0);
    rect(30, 20, 240, 60);
    strokeWeight(8);
    stroke(0, 0, 255);
    line(50, 50, 250, 50);
    line(145  ,40, 145, 60);
    stroke(255, 0, 255);
    line(mouseX,25, mouseX, 75);
    sd1=int(map(mouseX, 50, 250, -100, 100));
    fill(0);
    noStroke();
    rect(345, 25, 150, 50);
    fill(255);
    textSize(50);
    text(sd1, 350, 70);
  }
  if (mouseX>=50 && mouseX<=250  && mouseY>=125 && mouseY<=175)
  {
    noStroke();
    fill(0);
    rect(30, 120, 240, 60);
    strokeWeight(8);
    stroke(0, 0, 255);
    line(50, 150, 250, 150);
    line(145  ,140, 145, 160);
    stroke(255, 0, 255);
    line(mouseX, 125, mouseX, 175);
    sd2=int(map(mouseX, 50, 250, -100, 100));
    fill(0);
    noStroke();
    rect(340, 125, 150, 50);
    fill(255);
    textSize(50);
    text(sd2, 350, 170);
  }
  if (mouseX>=50 && mouseX<=250  && mouseY>=225 && mouseY<=275)
  {
    noStroke();
    fill(0);
    rect(30, 220, 240, 60);
    strokeWeight(8);
    stroke(0, 0, 255);
    line(50, 250, 250, 250);
    line(145  ,240, 145, 260);
    stroke(255, 0, 255);
    line(mouseX, 225, mouseX, 275);
    sd3=int(map(mouseX, 50, 250, -100, 100));
    fill(0);
    noStroke();
    rect(340, 225, 150, 50);
    fill(255);
    textSize(50);
    text(sd3, 350, 270);
  }
  if (mouseX>=50 && mouseX<=250  && mouseY>=325 && mouseY<=375)
  {
    noStroke();
    fill(0);
    rect(30, 320, 240, 60);
    strokeWeight(8);
    stroke(0, 0, 255);
    line(50, 350, 250, 350);
    line(145  ,340, 145, 360);
    stroke(255, 0, 255);
    line(mouseX, 325, mouseX, 375);
    sd4=int(map(mouseX, 50, 250, -100, 100));
    fill(0);
    noStroke();
    rect(340, 325, 150, 50);
    fill(255);
    textSize(50);
    text(sd4, 350, 370);
  }
  if (mouseX>=50 && mouseX<=250  && mouseY>=425 && mouseY<=475)
  {
    noStroke();
    fill(0);
    rect(30, 420, 240, 60);
    strokeWeight(8);
    stroke(0, 0, 255);
    line(50, 450, 250, 450);
    line(145  ,440, 145, 460);
    stroke(255, 0, 255);
    line(mouseX, 425, mouseX, 475);
    sd5=int(map(mouseX, 50, 250, -100, 100));
    fill(0);
    noStroke();
    rect(340, 425, 150, 50);
    fill(255);
    textSize(50);
    text(sd5, 350, 470);
  }
  
/*   String sta = "M";
    String a = str(sd1);
    String b = str(sd2);
    String c = str(sd3);
    String com = ";";
    String newl = "\n";
    String send1 = sta + a + com + b + com + c + newl;
    port.write(send1);
  */  
    port.write( ""+sd1+";"+sd2+";"+sd3+";"+sd4+";"+sd5+";");

  
 
}

void serialEvent(Serial port)
{
    reception = port.read();
    print(char(reception)); 

}
