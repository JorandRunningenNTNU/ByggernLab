https://github.com/JorandRunningenNTNU/ByggernLab.git

På github er koden til node 1 i mappen "BigBeautifulBroject" og koden til node 2 er i "MamdanisNewYork".

# Fordeling mellom de ulike nodene
Node 2 sine oppgaver er å styre motoren, servoen, solenoiden, detektere når man mister et liv og å bruke motoren som en høytaler (vi bruker PWM-signalet til motoren til å lage musikk). Node 2 mottar meldinger over CAN fra node 1, disse meldingene innholder hvilken posisjon skyte-greia/servo-feste/hva-man-nå-en-kaller-greia skal kjøres til, hviken posisjon servoen skal være i, når solenoiden skal utløses, og når motoren skal spille av ulike lyder. Node 2 sender meldinger over CAN til node 1 når man mister et liv. 

Node 1 sine oppgaver er å hoste et meny-system, slik at man på IO-bordet kan velge hvilket program man ønsker å kjøre, f.eks spillet. Det er dessuten node 1 som har all spill-logikken.  


# Kode i node 1 - BigBeautifulBroject
Node 1 har et meny-system. Koden for den er  i "menu", dette gir en interaktiv meny man kan bruke fra IO-boradet. Programmer som skal legges inn i meny-systemet legges inn i "menu_builder". Spill-logikken finner du i "program/degame". For øverig har node 1 diverse drivere for å utføre tingene spill-logikken trenger. 

"main" for node 1 initialiserer først alt som må initialiseres. Deretter er det en while-løkke som tar inn kommandoer fra IO-boardet og oppdaterer meny-skjermen basert på dette. Selve spillet kjøres altså fra inni menyen. 


# Kode i node 2 - MamdanisNewYork
Koden i node 2 er i stor grad bassert på interrupt. I "main" gjøres først initialisering, deretter er det en while-løkke, og det eneste while-løkken gjør er å spille av musikk når man dør. Alt annet er bassert på interrupt. 

Motoren styres med en PID-regulator som du finner i "motor_controller". Denne oppdaterer motorpådraget med en frekvens på 1 kHz. Hele oppdateringen av pådraget er i en interrupt-service-routine for en timer. Oppsettet av timeren er i "drivere/our_timer". 

Det er dessuten en intrrupt som reagerer på CAN-meldinger, og reagerer i henhold til hva can-meldingen er. Dette finner du i "can_interface". 

Dessuten er det en adc-interrupt. Denne interrupten trigges når adc-en måler en spenning som er under en gitt grense. Dette skjer derimot ganske ofte, pga. støy, så vi har lagt til et filter som gjør at adc-interrupten må trigges 100_000 ganger før det registerers som et tapt liv. adc-en sampler derimot med rundt 100 kHz, så dette tar typ 1 sekund fra ballen treffer platen bakerst i spillet. Dette er i "driver/adc"

Forøverig inneholder node 2 mange drivere for å få de nevnte interuptene til å gjøre tingene node 2 skal gjøre. 