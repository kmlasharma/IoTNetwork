#
# OMNeT++/OMNEST Makefile for IoTNetwork
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -KINET_PROJ=/Users/KmlaSharma/Downloads/inet -I. -I$$\(INET_PROJ\)/src -L$$\(INET_PROJ\)/out/$$\(CONFIGNAME\)/src -lINET
#

# Name of target to be created (-o option)
TARGET = IoTNetwork$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -I$(INET_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET_PROJ)/out/$(CONFIGNAME)/src  -lINET

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/IntermediateRouter.o \
    $O/IoTClient.o \
    $O/LogGenerator.o \
    $O/Router.o \
    $O/Server.o \
    $O/AggregatedPacket_m.o \
    $O/CoAP_m.o \
    $O/IoTPacket_m.o

# Message files
MSGFILES = \
    AggregatedPacket.msg \
    CoAP.msg \
    IoTPacket.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET_PROJ=/Users/KmlaSharma/Downloads/inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(TOOLCHAIN_NAME),clangc2)
LIBS += -Wl,-rpath,$(abspath $(INET_PROJ)/out/$(CONFIGNAME)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f IoTNetwork IoTNetwork.exe libIoTNetwork.so libIoTNetwork.a libIoTNetwork.dll libIoTNetwork.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f Logs/*_m.cc Logs/*_m.h Logs/*_sm.cc Logs/*_sm.h
	$(Q)-rm -f Logs/10sec/*_m.cc Logs/10sec/*_m.h Logs/10sec/*_sm.cc Logs/10sec/*_sm.h
	$(Q)-rm -f Logs/4sec, 0.2/*_m.cc Logs/4sec, 0.2/*_m.h Logs/4sec, 0.2/*_sm.cc Logs/4sec, 0.2/*_sm.h
	$(Q)-rm -f Logs/5sec, 0.2/*_m.cc Logs/5sec, 0.2/*_m.h Logs/5sec, 0.2/*_sm.cc Logs/5sec, 0.2/*_sm.h
	$(Q)-rm -f Logs/6sec, 0.2/*_m.cc Logs/6sec, 0.2/*_m.h Logs/6sec, 0.2/*_sm.cc Logs/6sec, 0.2/*_sm.h
	$(Q)-rm -f Logs/7sec, 0.2/*_m.cc Logs/7sec, 0.2/*_m.h Logs/7sec, 0.2/*_sm.cc Logs/7sec, 0.2/*_sm.h
	$(Q)-rm -f Logs/8sec, 0.2/*_m.cc Logs/8sec, 0.2/*_m.h Logs/8sec, 0.2/*_sm.cc Logs/8sec, 0.2/*_sm.h
	$(Q)-rm -f Logs/9sec, 0.2/*_m.cc Logs/9sec, 0.2/*_m.h Logs/9sec, 0.2/*_sm.cc Logs/9sec, 0.2/*_sm.h
	$(Q)-rm -f scripts/*_m.cc scripts/*_m.h scripts/*_sm.cc scripts/*_sm.h
	$(Q)-rm -f scripts/OLD/*_m.cc scripts/OLD/*_m.h scripts/OLD/*_sm.cc scripts/OLD/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/*_m.cc scripts/OLD/Varied Congestion/*_m.h scripts/OLD/Varied Congestion/*_sm.cc scripts/OLD/Varied Congestion/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.2/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.2/*_m.h scripts/OLD/Varied Congestion/FREQ 0.2/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.2/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.3s/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.3s/*_m.h scripts/OLD/Varied Congestion/FREQ 0.3s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.3s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.3s/Simulation_2017-03-22_19:47:27/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.3s/Simulation_2017-03-22_19:47:27/*_m.h scripts/OLD/Varied Congestion/FREQ 0.3s/Simulation_2017-03-22_19:47:27/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.3s/Simulation_2017-03-22_19:47:27/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/100_Simulation_2017-03-28_18:36:35/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/100_Simulation_2017-03-28_18:36:35/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/100_Simulation_2017-03-28_18:36:35/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/100_Simulation_2017-03-28_18:36:35/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/200_Simulation_2017-03-28_18:37:46/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/200_Simulation_2017-03-28_18:37:46/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/200_Simulation_2017-03-28_18:37:46/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/200_Simulation_2017-03-28_18:37:46/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/300_Simulation_2017-03-20_17:08:28/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/300_Simulation_2017-03-20_17:08:28/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/300_Simulation_2017-03-20_17:08:28/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/300_Simulation_2017-03-20_17:08:28/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/400_Simulation_2017-03-28_18:38:52/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/400_Simulation_2017-03-28_18:38:52/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/400_Simulation_2017-03-28_18:38:52/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/400_Simulation_2017-03-28_18:38:52/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/500_Simulation_2017-03-20_17:08:51/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/500_Simulation_2017-03-20_17:08:51/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/500_Simulation_2017-03-20_17:08:51/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/500_Simulation_2017-03-20_17:08:51/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/600_Simulation_2017-03-28_18:40:06/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/600_Simulation_2017-03-28_18:40:06/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/600_Simulation_2017-03-28_18:40:06/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/600_Simulation_2017-03-28_18:40:06/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/700_Simulation_2017-03-28_18:44:00/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/700_Simulation_2017-03-28_18:44:00/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/700_Simulation_2017-03-28_18:44:00/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/700_Simulation_2017-03-28_18:44:00/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/800_Simulation_2017-03-28_18:46:36/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/800_Simulation_2017-03-28_18:46:36/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/800_Simulation_2017-03-28_18:46:36/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/800_Simulation_2017-03-28_18:46:36/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.4s/900_Simulation_2017-03-20_17:08:58/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.4s/900_Simulation_2017-03-20_17:08:58/*_m.h scripts/OLD/Varied Congestion/FREQ 0.4s/900_Simulation_2017-03-20_17:08:58/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.4s/900_Simulation_2017-03-20_17:08:58/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.5s/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.5s/*_m.h scripts/OLD/Varied Congestion/FREQ 0.5s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.5s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.5s/Simulation_2017-03-22_19:32:58/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.5s/Simulation_2017-03-22_19:32:58/*_m.h scripts/OLD/Varied Congestion/FREQ 0.5s/Simulation_2017-03-22_19:32:58/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.5s/Simulation_2017-03-22_19:32:58/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.6/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.6/*_m.h scripts/OLD/Varied Congestion/FREQ 0.6/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.6/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.7s/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.7s/*_m.h scripts/OLD/Varied Congestion/FREQ 0.7s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.7s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.7s/Simulation_2017-03-22_19:30:54/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.7s/Simulation_2017-03-22_19:30:54/*_m.h scripts/OLD/Varied Congestion/FREQ 0.7s/Simulation_2017-03-22_19:30:54/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.7s/Simulation_2017-03-22_19:30:54/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.8/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.8/*_m.h scripts/OLD/Varied Congestion/FREQ 0.8/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.8/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 0.9/*_m.cc scripts/OLD/Varied Congestion/FREQ 0.9/*_m.h scripts/OLD/Varied Congestion/FREQ 0.9/*_sm.cc scripts/OLD/Varied Congestion/FREQ 0.9/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 1.5s/*_m.cc scripts/OLD/Varied Congestion/FREQ 1.5s/*_m.h scripts/OLD/Varied Congestion/FREQ 1.5s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 1.5s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 1.5s/Simulation_2017-03-22_19:08:31/*_m.cc scripts/OLD/Varied Congestion/FREQ 1.5s/Simulation_2017-03-22_19:08:31/*_m.h scripts/OLD/Varied Congestion/FREQ 1.5s/Simulation_2017-03-22_19:08:31/*_sm.cc scripts/OLD/Varied Congestion/FREQ 1.5s/Simulation_2017-03-22_19:08:31/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 1s/*_m.cc scripts/OLD/Varied Congestion/FREQ 1s/*_m.h scripts/OLD/Varied Congestion/FREQ 1s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 1s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 1s/Simulation_2017-03-22_19:04:34/*_m.cc scripts/OLD/Varied Congestion/FREQ 1s/Simulation_2017-03-22_19:04:34/*_m.h scripts/OLD/Varied Congestion/FREQ 1s/Simulation_2017-03-22_19:04:34/*_sm.cc scripts/OLD/Varied Congestion/FREQ 1s/Simulation_2017-03-22_19:04:34/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 2.5s/*_m.cc scripts/OLD/Varied Congestion/FREQ 2.5s/*_m.h scripts/OLD/Varied Congestion/FREQ 2.5s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 2.5s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 2.5s/Simulation_2017-03-22_19:14:42/*_m.cc scripts/OLD/Varied Congestion/FREQ 2.5s/Simulation_2017-03-22_19:14:42/*_m.h scripts/OLD/Varied Congestion/FREQ 2.5s/Simulation_2017-03-22_19:14:42/*_sm.cc scripts/OLD/Varied Congestion/FREQ 2.5s/Simulation_2017-03-22_19:14:42/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 2s/*_m.cc scripts/OLD/Varied Congestion/FREQ 2s/*_m.h scripts/OLD/Varied Congestion/FREQ 2s/*_sm.cc scripts/OLD/Varied Congestion/FREQ 2s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Congestion/FREQ 2s/Simulation_2017-03-22_19:13:00/*_m.cc scripts/OLD/Varied Congestion/FREQ 2s/Simulation_2017-03-22_19:13:00/*_m.h scripts/OLD/Varied Congestion/FREQ 2s/Simulation_2017-03-22_19:13:00/*_sm.cc scripts/OLD/Varied Congestion/FREQ 2s/Simulation_2017-03-22_19:13:00/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Datarate/*_m.cc scripts/OLD/Varied Datarate/*_m.h scripts/OLD/Varied Datarate/*_sm.cc scripts/OLD/Varied Datarate/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Datarate/100mbps_FREQ 0.3s/*_m.cc scripts/OLD/Varied Datarate/100mbps_FREQ 0.3s/*_m.h scripts/OLD/Varied Datarate/100mbps_FREQ 0.3s/*_sm.cc scripts/OLD/Varied Datarate/100mbps_FREQ 0.3s/*_sm.h
	$(Q)-rm -f scripts/OLD/Varied Datarate/54mbps_FREQ 0.3s/*_m.cc scripts/OLD/Varied Datarate/54mbps_FREQ 0.3s/*_m.h scripts/OLD/Varied Datarate/54mbps_FREQ 0.3s/*_sm.cc scripts/OLD/Varied Datarate/54mbps_FREQ 0.3s/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/*_m.cc scripts/VARIED WAITING TIME (0.2s)/*_m.h scripts/VARIED WAITING TIME (0.2s)/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/10secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/10secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/10secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/10secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/4secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/4secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/4secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/4secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/5secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/5secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/5secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/5secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/6secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/6secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/6secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/6secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/7secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/7secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/7secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/7secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/8secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/8secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/8secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/8secs/*_sm.h
	$(Q)-rm -f scripts/VARIED WAITING TIME (0.2s)/9secs/*_m.cc scripts/VARIED WAITING TIME (0.2s)/9secs/*_m.h scripts/VARIED WAITING TIME (0.2s)/9secs/*_sm.cc scripts/VARIED WAITING TIME (0.2s)/9secs/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%.o=%.d)
