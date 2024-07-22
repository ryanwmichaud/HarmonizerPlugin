# Harmonizer Plugin

Explore non functional constant structure harmony one note at a time

## Description

A midi plugin that harmonizes incoming melody notes by cycling through every inversion of a user provided chord. <br>
Check it out if you're looking to explore some more modern sounding harmony for your beats, reharmonizations, or practice inspiration!

**Example:** <br>
  - Chordtones: 1, 2, M3, P5, M7  (Major9 chord)
  - Note to harmonize: **C**
  <br> <br>
  - Possible output results:
    - D,E,G,B,**C**    (CMajor9)
    - D,F,A,Bb,**C**    (BbMajor9)
    - Eb,G,Ab,Bb,**C**  (AbMajor9)
    - E,F,G,A,**C**    (FMajor9)
    - Db,Eb,F,Ab,**C**  (DbMajor9)

## To Install:
 - Go to the releases section on the right and download the .vst3 file for your operating system/archetecture from the latest release
 - Copy the .vst3 file into your VST3 folder
  
## To Use the Plugin:
This is a midi plugin and requires a DAW with midi routing capabilities. The plugin is loaded onto a midi track whose output is routed into a seperate midi track with your software instrument of choice.

### Ableton:
Create two new tracks and arm them <br>
- Track One - Plugin
  - Load the plugin onto this track
- Track Two - Software Instrument
  - Load your MIDI instrument plugin onto this track
  - Set the input as track one
### Reaper:
Create two new tracks <br>
- Track One - Plugin
  - Load the plugin onto this track
  - Set the input to be from your midi keyboard <br>
- Track Two - Software Instrument
  - Load your MIDI instrument plugin onto this track
  - Set the input as NONE 
  - Under routing options, create a new recieve from track one

See the screenshots/demos folder for examples

## To Develop/Build From Source:
- Install the JUCE library, C++ development tools, and this repository
- Open the `HarmonizerPlugin.jucer` file in Projucer (part of the JUCE install) 
- Use Projucer to export the project into your IDE of choice
- Build the VST3 solution and copy the resulting `HarmonizerPlugin.vst3` file from your chosen output directory to your VST3 folder

## To Run Unit Tests:
- Ensure you have the JUCE library and C++ development tools installed
- Open `UnitTesting/UnitTesting.jucer` in Projucer (you will need to build a seperate Console App to run the tests)
- Use Projucer to export the UnitTesting project into your IDE of choice
- Add any new tests you'd like to run to `Main.cpp`
- Build the Console App solution to run the tests
<br>
Feel free to reach out to ryanwilliammichaud@gmail.com with any additional questions or to report a bug

