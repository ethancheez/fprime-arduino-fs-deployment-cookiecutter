// ======================================================================
// \title  {{cookiecutter.deployment_name}}Topology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <{{cookiecutter.deployment_name}}/Top/{{cookiecutter.deployment_name}}TopologyAc.hpp>
#include <{{cookiecutter.deployment_name}}/Top/{{cookiecutter.deployment_name}}PacketsAc.hpp>
#include <config/FppConstantsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace {{cookiecutter.deployment_name}};

// The reference topology uses a malloc-based allocator for components that need to allocate memory during the
// initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1/100Hz, 1/200Hz, and 1/1000Hz
{{"Svc::RateGroupDriver::DividerSet rateGroupDivisors{{{100, 0}, {200, 0}, {1000, 0}}};"}}

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    COM_BUFFER_SIZE   = 140,
    COM_BUFFER_COUNT  = 3,
    BUFFER_MANAGER_ID = 200,
    FILE_DOWNLINK_TIMEOUT = 1000,
    FILE_DOWNLINK_COOLDOWN = 1000,
    FILE_DOWNLINK_CYCLE_TIME = 1000,
    FILE_DOWNLINK_FILE_QUEUE_DEPTH = 10,
    UPLINK_BUFFER_MANAGER_STORE_SIZE = 1024,
    UPLINK_BUFFER_MANAGER_QUEUE_SIZE = 30,
    UPLINK_BUFFER_MANAGER_ID = 100,
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisors);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));

    // File downlink requires some project-derived properties.
    fileDownlink.configure(FILE_DOWNLINK_TIMEOUT, FILE_DOWNLINK_COOLDOWN, FILE_DOWNLINK_CYCLE_TIME,
                           FILE_DOWNLINK_FILE_QUEUE_DEPTH);

    // Set up BufferManager
    Svc::BufferManager::BufferBins buffMgrBins;
    memset(&buffMgrBins, 0, sizeof(buffMgrBins));
    buffMgrBins.bins[0].bufferSize = COM_BUFFER_SIZE;
    buffMgrBins.bins[0].numBuffers = COM_BUFFER_COUNT;
    bufferManager.setup(BUFFER_MANAGER_ID, 0, mallocator, buffMgrBins);

    // Set up Uplink BufferManager
    Svc::BufferManager::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins, 0, sizeof(upBuffMgrBins));
    upBuffMgrBins.bins[0].bufferSize = UPLINK_BUFFER_MANAGER_STORE_SIZE;
    upBuffMgrBins.bins[0].numBuffers = UPLINK_BUFFER_MANAGER_QUEUE_SIZE;
    fileUplinkBufferManager.setup(UPLINK_BUFFER_MANAGER_ID, 0, mallocator, upBuffMgrBins);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);
}

// Public functions for use in main program are namespaced with deployment name {{cookiecutter.deployment_name}}
namespace {{cookiecutter.deployment_name}} {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    // loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
    
    rateDriver.configure(1);
    commDriver.configure(&Serial);
    rateDriver.start();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);
}
};  // namespace {{cookiecutter.deployment_name}}
