#!/bin/bash

export NO_OF_EVENTS_PER_RUN=500
export NO_OF_SIGNAL_PLOTS=10
export NO_OF_SIGNALS_SAVED=1000

export TIME_MAX=500  # In ms
export TIME_MIN=-10  # In ms
export TIME_RESOLUTION=0.1 # In ms

export TES_OP_TEMP=41e-3  # In kelvin

export I_BIAS=26e-6  # In amps
export R_SHUNT=2e-3  # In ohms
export R_PARASITIC=1e-4  # In ohms

export TES_INDUCTOR_WINDINGS=1
export TES_INDUCTOR_RADIUS=0.1  # In m

export THERMAL_CONTACT_INEFFICIENCY=1e-5

export SQUID_FB_RESISTANCE=1e6  # In ohms
export SQUID_FB_VOLTAGE=10  # In volts
export SQUID_FB_INDUCTOR_WINDINGS=1
export SQUID_FB_INDUCTOR_RADIUS=0.1  # In m
export SQUID_SLEW_RATE=2e6  # In magnetic quantum per second
export OUTPUT_FILENAME="Histos.root"
