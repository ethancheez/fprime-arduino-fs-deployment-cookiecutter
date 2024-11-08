module {{cookiecutter.deployment_name}} {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
    }

  topology {{cookiecutter.deployment_name}} {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance bufferManager
    instance cmdDisp
    instance commDriver
    instance deframer
    instance eventLogger
    instance fatalHandler
    instance fileDownlink
    instance fileManager
    instance fileUplink
    instance fileUplinkBufferManager
    instance framer
    instance rateDriver
    instance rateGroup1
    instance rateGroupDriver
    instance systemResources
    instance textLogger
    instance timeHandler
    instance tlmSend

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance timeHandler

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rateDriver.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> commDriver.schedIn
      rateGroup1.RateGroupMemberOut[1] -> tlmSend.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run
      rateGroup1.RateGroupMemberOut[3] -> fileDownlink.Run
    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections Downlink {

      tlmSend.PktSend -> framer.comIn
      eventLogger.PktSend -> framer.comIn
      fileDownlink.bufferSendOut -> framer.bufferIn

      framer.framedAllocate -> bufferManager.bufferGetCallee
      framer.framedOut -> commDriver.$send
      framer.bufferDeallocate -> fileDownlink.bufferReturn

      commDriver.deallocate -> bufferManager.bufferSendIn

    }
    
    connections Uplink {

      commDriver.allocate -> bufferManager.bufferGetCallee
      commDriver.$recv -> deframer.framedIn
      deframer.framedDeallocate -> bufferManager.bufferSendIn

      deframer.comOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> fileUplinkBufferManager.bufferGetCallee
      deframer.bufferOut -> fileUplink.bufferSendIn
      deframer.bufferDeallocate -> fileUplinkBufferManager.bufferSendIn
      fileUplink.bufferSendOut -> fileUplinkBufferManager.bufferSendIn
      
    }

    connections {{cookiecutter.deployment_name}} {
      # Add here connections to user-defined components
    }

  }

}
