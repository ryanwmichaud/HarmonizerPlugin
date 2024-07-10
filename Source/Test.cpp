#include "Test.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"


class Test : public juce::UnitTest
{
public:
    Test() : UnitTest("Testing") {}

    void runTest() override
    {
        beginTest("Processor");

        HarmonizerPluginAudioProcessor processor;
        expectEquals(processor.getNumPrograms(),1);

        beginTest("Editor");

        HarmonizerPluginAudioProcessorEditor editor(processor);
        expect(editor.getWidth() > 0);

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