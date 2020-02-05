# Bikaya Phase 1

Il pacchetto scaricato contiene la fase 1 del progetto Bikaya (http://www.cs.unibo.it/~renzo/so/bikaya/phase1_2020.pdf) inerente il corso di Sistemi Operativi per l'AA 2019/2020.
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

## Esecuzione (credit to Mattia Maldini)

Per l'esecuzione di Phase1 fare riferimento ai manuali di uARM e uMPS2, rispettivamente.
