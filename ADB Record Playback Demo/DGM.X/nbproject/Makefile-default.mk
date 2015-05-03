#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/Font25PixelsHeight.c ../src/icons_adb.c ../src/MainDemo.c ../src/WM8960Drv.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o ${OBJECTDIR}/_ext/1360937237/icons_adb.o ${OBJECTDIR}/_ext/1360937237/MainDemo.o ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d ${OBJECTDIR}/_ext/1360937237/icons_adb.o.d ${OBJECTDIR}/_ext/1360937237/MainDemo.o.d ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o ${OBJECTDIR}/_ext/1360937237/icons_adb.o ${OBJECTDIR}/_ext/1360937237/MainDemo.o ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o

# Source Files
SOURCEFILES=../src/Font25PixelsHeight.c ../src/icons_adb.c ../src/MainDemo.c ../src/WM8960Drv.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o: ../src/Font25PixelsHeight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d" -o ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o ../src/Font25PixelsHeight.c  
	
${OBJECTDIR}/_ext/1360937237/icons_adb.o: ../src/icons_adb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/icons_adb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/icons_adb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/icons_adb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/icons_adb.o.d" -o ${OBJECTDIR}/_ext/1360937237/icons_adb.o ../src/icons_adb.c  
	
${OBJECTDIR}/_ext/1360937237/MainDemo.o: ../src/MainDemo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/MainDemo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/MainDemo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/MainDemo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/MainDemo.o.d" -o ${OBJECTDIR}/_ext/1360937237/MainDemo.o ../src/MainDemo.c  
	
${OBJECTDIR}/_ext/1360937237/WM8960Drv.o: ../src/WM8960Drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d" -o ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o ../src/WM8960Drv.c  
	
else
${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o: ../src/Font25PixelsHeight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o.d" -o ${OBJECTDIR}/_ext/1360937237/Font25PixelsHeight.o ../src/Font25PixelsHeight.c  
	
${OBJECTDIR}/_ext/1360937237/icons_adb.o: ../src/icons_adb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/icons_adb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/icons_adb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/icons_adb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/icons_adb.o.d" -o ${OBJECTDIR}/_ext/1360937237/icons_adb.o ../src/icons_adb.c  
	
${OBJECTDIR}/_ext/1360937237/MainDemo.o: ../src/MainDemo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/MainDemo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/MainDemo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/MainDemo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/MainDemo.o.d" -o ${OBJECTDIR}/_ext/1360937237/MainDemo.o ../src/MainDemo.c  
	
${OBJECTDIR}/_ext/1360937237/WM8960Drv.o: ../src/WM8960Drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -I"../h" -I"." -O3 -funroll-loops -MMD -MF "${OBJECTDIR}/_ext/1360937237/WM8960Drv.o.d" -o ${OBJECTDIR}/_ext/1360937237/WM8960Drv.o ../src/WM8960Drv.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../libgfxusbmsdPIC32MX795F512L_ADK.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   ..\libgfxusbmsdPIC32MX795F512L_ADK.a    -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=24000,--defsym=_min_stack_size=2048,-L"..",-L".",-Map="${DISTDIR}/DGM.X.${IMAGE_TYPE}.map" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../libgfxusbmsdPIC32MX795F512L_ADK.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   ..\libgfxusbmsdPIC32MX795F512L_ADK.a    -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=24000,--defsym=_min_stack_size=2048,-L"..",-L".",-Map="${DISTDIR}/DGM.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/DGM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
