#include  <JuceHeader.h>
#include "TestProcessor.h" 
#include "Functions.h"

class Test : public juce::UnitTest
{
public:
    Test() : UnitTest("Testing") {}

   

    void runTest() override
    {
        
        beginTest("addChordTone");
        MidiProcessor mp;
        mp.addChordTone(4);
        mp.addChordTone(7);
        expect(mp.numCTs == 3, "Expected: " + juce::String(3) + ", Actual: " + juce::String(mp.numCTs));
        expect(arrEq(mp.chord, {1,0,0,0,1,0,0,1,0,0,0,0}), "Expected: " + juce::String(chordToString({1,0,0,0,1,0,0,1,0,0,0,0})) + ", Actual: " + juce::String(chordToString(mp.chord)));
        mp.addChordTone(7); //handles redundant adds?
        expect(arrEq(mp.chord, { 1,0,0,0,1,0,0,1,0,0,0,0 }), "Expected: " + juce::String(chordToString({ 1,0,0,0,1,0,0,1,0,0,0,0 })) + ", Actual: " + juce::String(chordToString(mp.chord)));
        mp.addChordTone(11);
        expect(arrEq(mp.chord, { 1,0,0,0,1,0,0,1,0,0,0,1 }), "Expected: " + juce::String(chordToString({ 1,0,0,0,1,0,0,1,0,0,0,1 })) + ", Actual: " + juce::String(chordToString(mp.chord)));
        expect(mp.numCTs == 4, "Expected: " + juce::String(4) + ", Actual: " + juce::String(mp.numCTs));



        beginTest("removeChordTone");
        mp.removeChordTone(11);
        expect(mp.numCTs == 3, "Expected: " + juce::String(3) + ", Actual: " + juce::String(mp.numCTs));
        expect(arrEq({ 1,0,0,0,1,0,0,1,0,0,0,0 }, mp.chord), "Expected: " + juce::String(chordToString({ 1,0,0,0,1,0,0,1,0,0,0,0 })) + ", Actual: " + juce::String(chordToString(mp.chord)) );
        mp.removeChordTone(11);
        expect(arrEq({ 1,0,0,0,1,0,0,1,0,0,0,0 }, mp.chord), "Expected: " + juce::String(chordToString({ 1,0,0,0,1,0,0,1,0,0,0,0 })) + ", Actual: " + juce::String(chordToString(mp.chord)));
        expect(mp.numCTs == 3, "Expected: " + juce::String(3) + ", Actual: " + juce::String(mp.numCTs));

        
        
        beginTest("inversions");
        mp.updateChord();
        expect(mp.numCTs == 3, "Expected: " + juce::String(3) + ", Actual: " + juce::String(mp.numCTs));

        std::string actual = inversionToString(mp.inversions[0], mp.numCTs);
        std::string expected = "[0, -8, -5]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[1], mp.numCTs);
        expected = "[0, -9, -4]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[2], mp.numCTs);
        expected = "[0, -7, -3]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));


        
        
        beginTest("update to bigger chord");
        mp.addChordTone(2);
        mp.addChordTone(11);
        mp.updateChord();

        expect(mp.numCTs == 5, "Expected: " + juce::String(5) + ", Actual: " + juce::String(mp.numCTs));

        actual = inversionToString(mp.inversions[0], mp.numCTs);
        expected = "[0, -10, -8, -5, -1]";

        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[1], mp.numCTs);
        expected = "[0, -10, -7, -3, -2]";

        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[2], mp.numCTs);
        expected = "[0, -9, -5, -4, -2]";

        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[3], mp.numCTs);
        expected = "[0, -8, -7, -5, -3]";

        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[4], mp.numCTs);
        expected = "[0, -11, -9, -7, -4]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));






        beginTest("update to smaller chord");
        mp.removeChordTone(2);
        mp.removeChordTone(11);
        mp.removeChordTone(4);
        mp.updateChord();

        expect(mp.numCTs == 2, "Expected: " + juce::String(2) + ", Actual: " + juce::String(mp.numCTs));

        actual = inversionToString(mp.inversions[0], mp.numCTs);
        expected = "[0, -5]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        actual = inversionToString(mp.inversions[1], mp.numCTs);
        expected = "[0, -7]";
        expect(expected == actual, "Expected: " + juce::String(expected) + ", Actual: " + juce::String(actual));

        


    }




};

// Creating a static instance will automatically add the instance to the array
// returned by UnitTest::getAllTests(), so the test will be included when you call
// UnitTestRunner::runAllTests()
static Test test;




int main(int argc, char** argv)
{
    juce::UnitTestRunner runner;
    runner.runAllTests();
    return 0;
}