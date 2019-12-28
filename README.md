# Bikaya Phase 0

Il pacchetto scaricato contiene la fase 0 del progetto Bikaya (http://www.cs.unibo.it/~renzo/so/bikaya/phase0_2020.pdf) inerente il corso di Sistemi Operativi per l'AA 2019/2020.
Il pacchetto contiene un programma compilabile sia per l'emulatore uMPS2 (https://github.com/tjonjic/umps) che uARM (https://github.com/mellotanica/uARM). 

L'architettura è configurata tramite metodologia make.

## Requisiti (credit to Mattia Maldini)

Perche' la compilazione vada a buon fine sono necessari i seguenti pacchetti:

- arm-none-eabi-gcc
- mipsel-linux-gnu-gcc
- uarm (per la compilazione su uarm)
- umps (per la compilazione su umps)
- make (per utilizzare i makefile)

## Istruzioni per la compilazione

Sono forniti due makefile per la compilazione, `uarmmake` e `umpsmake`. Invocando `make uarm` o `make umps2` si procede con la compilazione dell'architettura richiesta.

## Prerequisiti per l'esecuzione

Assicurarsi che esistano i file relativi al device di stampa e al terminale.
Se non esistono, crearli e aggiungerli alla configurazione delle macchine.

eg: 
    $touch term0.uarm
    $touch term0.umps
    $touch prin0.uarm
    $touch prin0.umps 

## Esecuzione (credit to Mattia Maldini)

Per l'esecuzione dell'esempio fare riferimento ai manuali di uARM e uMPS2, rispettivamente.
