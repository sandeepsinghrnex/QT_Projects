/**
   @file GEC_ISfpdpRecorder.h
 */

#ifndef GEC_ISFPDPRECORDER_H_
#define GEC_ISFPDPRECORDER_H_

#include <string>
#include <stdint.h>

#define GEC_SFPDP_RECORDER_VERSION "3.5.0"

#define GEC_SFPDP_RECORDER_SERVER_UNIX_SOCKET_FILE "/tmp/ge-sfpdp-server"
#define GEC_SFPDP_RECORDER_SERVER_TCP_PORT 8001

enum {
  GEC_SFPDP_1_0625_GBPS,
  GEC_SFPDP_2_125_GBPS,
  GEC_SFPDP_2_5_GBPS,
  GEC_SFPDP_3_125_GBPS,
  GEC_SFPDP_4_25_GBPS
};

#define GEC_SFPDP_NO_WAIT_FOR_SYNC  0
#define GEC_SFPDP_WAIT_FOR_ANY_SYNC  1

#define GEC_SFPDP_PLAYBACK_WITHOUT_SIGNALS       0
#define GEC_SFPDP_PLAYBACK_WITH_UNTIMED_SIGNALS  1
#define GEC_SFPDP_PLAYBACK_WITH_TIMED_SIGNALS    2

#define GEC_SFPDP_512_WORDS_CYCLE  0
#define GEC_SFPDP_16K_WORDS_CYCLE  1
#define GEC_SFPDP_512K_WORDS_CYCLE 2
#define GEC_SFPDP_16M_WORDS_CYCLE  3

#define GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_0  (1 << 0)
#define GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_1  (1 << 1)
#define GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_2  (1 << 2)
#define GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_3  (1 << 3)
#define GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT0      (1 << 4)
#define GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT1      (1 << 5)
#define GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT2      (1 << 6)
#define GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT3      (1 << 7)

#define GEC_SFPDP_AUTOMATIC_CONTROL  0
#define GEC_SFPDP_MANUAL_CONTROL     1

#define GEC_SFPDP_START_CHANNEL_SYNC  0
#define GEC_SFPDP_PAUSE_CHANNEL_SYNC  1
#define GEC_SFPDP_STOP_CHANNEL_SYNC   2

/**
   @interface GEC_ISfpdpRecorder

   Interface representing the API available for
   remote control of SFPDP recording.
*/
class GEC_ISfpdpRecorder
{
public:

  /**
     Set control mode for all internal channel sync masters

     This method configures how the internal channel sync masters are
     controlled.

     If the @p mode argument has the value @ref
     GEC_SFPDP_AUTOMATIC_CONTROL, @ref startAll() implicitly starts
     signal generation on all internal masters that one or more
     sessions are configured to use (see @ref setChannelSyncSource()).
     In this mode, it is not possible to pause the signal generation.

     If the @p mode argument has the value @ref
     GEC_SFPDP_MANUAL_CONTROL, the signal generation on all internal
     channel sync masters has to be explictly started by calling @ref
     setIntChannelSync(). The same method can also be called to pause
     or stop the signal generation.

     By default all internal channel sync masters are automatically
     controlled.

     @param mode
     integer whose value is @ref GEC_SFPDP_AUTOMATIC_CONTROL or @ref
     GEC_SFPDP_MANUAL_CONTROL


     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value value of @p mode isn't a valid control mode
   */
  virtual int setIntChannelSyncControl(int mode) = 0;

  /**
     Set state of internal channel sync generation

     This method is called to start, pause, or stop signal generation
     on one or more internal channel sync masters on the SFPDP module
     identified by the @p iDevice argument. The constants in the table
     below are used to identify the internal masters in the @p
     masterMask argument. Multiple masters are selected, by bitwise
     ORing together the corresponding constants in the @p masterMask
     argument.

     Source ID                                     | Description
     ----------------------------------------------|------------:
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_0 | Internal Master 0
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_1 | Internal Master 1
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_2 | Internal Master 2
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_3 | Internal Master 3
  
     Prior to calling this method, the recorder must have been
     configured to use manual control for the internal channel sync
     masters (see @ref setIntChannelSyncControl()).

     Signal generation from the selected masters is activated, when
     this method is called with @ref GEC_SFPDP_START_CHANNEL_SYNC as
     the 'action' parameter. And it is paused or stopped when this
     method is called with respectively @ref
     GEC_SFPDP_PAUSE_CHANNEL_SYNC or @ref GEC_SFPDP_STOP_CHANNEL_SYNC
     as the @p action parameter.


     @param iDevice
     32 bit integer giving the index of the SFPDP device to use. Most
     systems have only one device, so this value is usually 0.
     @param masterMask
     integer whose value is one internal channel sync master ID or a
     bitwise combination of several IDs
     @param action
     integer whose value is @ref GEC_SFPDP_START_CHANNEL_SYNC, @ref
     GEC_SFPDP_PAUSE_CHANNEL_SYNC, or @ref GEC_SFPDP_STOP_CHANNEL_SYNC

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The @p masterMask argument represents at least one ID that
     doesn't belong to an internal master
     @return GE_NOT_PERMITTED
     Manual control of internal channel sync masters is not enabled
     @return GE_NOT_EXISTING
     The given device index doesn't refer to a physical device
  */
  virtual int setIntChannelSync(uint32_t iDevice, int masterMask, int action) = 0;


  /**
     Set the source that drives an external channel sync output signal

     This method is only applicable to SFPDP recorder systems that
     provide an external channel sync output signal. On these system
     this method is called to configure what source should drive the
     output signal.
     
     The output signal is specified by the @p iDevice argument
     specifying the SFPDP module, and the @p signal argument
     specifying the ID of one of the external signals:
 
     Signal ID                                     | Description
     ----------------------------------------------|------------:
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT0     | EXT0 Coax Output
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT1     | EXT1 Coax Output
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT2     | EXT2 Coax Output
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT3     | EXT3 Coax Output

     The source that shall drive the output signal is specified by the
     @p source argument whose value is the ID of an internal master or
     an external input:

     Source ID                                     | Description
     ----------------------------------------------|------------:
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_0 | Internal Master 0
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_1 | Internal Master 1
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_2 | Internal Master 2
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_3 | Internal Master 3
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT0     | EXT0 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT1     | EXT1 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT2     | EXT2 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT3     | EXT3 Coax Input

     By default Internal Master 0 is used to drive an external channel
     sync output.

     @param iDevice
     32 bit integer giving the index of the SFPDP device to use. Most
     systems have only one device, so this value is usually 0.
     @param signal
     integer whose value is one of the 4 IDs for external channel sync
     signals
     @param source
     integer whose value is one of the 8 IDs for channel sync signal
     source

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of @p signal isn't the ID of an external master or the
     value of @p source isn't the ID of a source
     @return GE_NOT_PERMITTED
     The specified signal isn't routed to an connector.
     @return GE_NOT_EXISTING
     The given device index doesn't refer to a physical device
   */
  virtual int setChannelSyncOutput(uint32_t iDevice, int signal, int source) = 0;

  /**
     Reset recorder

     This method will first stop all active sessions, and then delete
     all existing sessions.

     Applications that use this interface should call reset on startup.

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
   */
  virtual int reset() = 0;

  /**
     Create a new recording session

     When the session is started (see @ref startAll()), data from a
     given SFPDP channel are stored into a data file.

     The recorder also produces an index file and a meta data file
     for each session. The index file will store information about
     events that occur during the recording session, such as
     reception of SYNC and detection of CRC errors. The meta data file
     contains timing information which the recorder can use to play
     back the data.

     The three files associated with a recording session will be
     created in the folder specified by the 'path' parameter. Their
     file basenames are given by the 'name' parameter.  File
     extensions '.dat', '.idx', and '.meta' will be used for
     respectively the data file, the index file, and the meta data
     file.

     @param path
     String with the full name of the directory where the files
     associated with the recording will be stored
     @param name
     String with the base name of the recording
     @param iDevice
     32 bit integer giving the index of the SFPDP device to use. Most
     systems have only one device, so this value is usually 0.
     @param iPort 
     32 bit integer giving the index of the SFPDP port to use. This
     value is the same as the port number on the corresponding front
     panel label.
     @param id 
     32 bit integer reference which returns an unique value
     representing the recording session

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_FILE_IN_USE
     The value of 'name' is already in use as base name in 'path' by another recording session
     @return GE_BUSY
     The given device and channel index pair is already assigned to another recording session
     @return GE_NOT_EXISTING
     The given device and channel index pair doesn't refer to any physical device
     @return GE_NO_ACCESS
     Recording files cannot be created in the directory specified by the path parameter
   */
  virtual int create(std::string path,
		     std::string name,
		     uint32_t iDevice,
		     uint32_t iPort,
		     uint32_t& id) = 0;
  
  /**
     Open a new playback session

     A playback session will always use a data file, and it may use a
     metadata file. The basename of both files is specified by the
     'name' parameter, and both must be located in the folder
     specified by the 'path' parameter. The two files are
     distinguished by their file extension - the data file has a
     '.dat' extension and the metadata file has a '.meta' extension.

     When the session is started (see @ref startAll()), data from a
     data file is played back to the Serial FPDP port specified by the
     'iDevice' and 'iPort' parameters.
     
     The 'mode' parameter specifies how the recorder will handle SYNC,
     DIR, PIO1, and PIO2 signaling:

     - if 'mode' is @ref GEC_SFPDP_PLAYBACK_WITHOUT_SIGNALS, the data will be
       played back as fast as possible without signaling (all signals
       are deasserted during playback).

     - if 'mode' is @ref GEC_SFPDP_PLAYBACK_WITH_UNTIMED_SIGNALS, the
       recorder regenerates the state changes of the signals relative
       to the recorded data words. But the timing of the recorded
       signals are not preserved - the data words are just transmitted
       as fast as possible.

     - if 'mode' is @ref GEC_SFPDP_PLAYBACK_WITH_TIMED_SIGNALS, the recorder
       regenerates the state changes of the signals relative to the
       recorded timing information. This means that the playback will
       be paced in order to resemble the timing of the recorded data
       flow.

     A playback session requires a meta data file in order to
     regenerate signaling (@ref GEC_SFPDP_PLAYBACK_WITH_UNTIMED_SIGNALS or
     @ref GEC_SFPDP_PLAYBACK_WITH_TIMED_SIGNALS).

     @param path
     String with the full name of the folder where the files
     associated with the playback session are stored
     @param name
     String with the base name of the data file and the meta data file
     @param iDevice
     32 bit integer giving the index of the SFPDP device to use. Most
     systems have only one device, so this value is 0.
     @param iPort 
     32 bit integer giving the index of the SFPDP port to use. This
     value is the same as the port number on the corresponding front
     panel label.
     @param mode
     integer whose value is @ref GEC_SFPDP_PLAYBACK_WITHOUT_SIGNALS,
     @ref GEC_SFPDP_PLAYBACK_WITH_UNTIMED_SIGNALS, or
     @ref GEC_SFPDP_PLAYBACK_WITH_TIMED_SIGNALS
     @param id 
     32 bit integer reference which returns an unique value
     representing the playback session

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_FILE_IN_USE
     The specified values for 'name' and 'path' are already in use as
     basename and folder in a recording session
     @return GE_BUSY
     The given device index and channel index pair is already assigned
     to another playback session
     @return GE_NOT_EXISTING
     The given device index and channel index pair doesn't refer to
     any physical device
     @return GE_NO_ACCESS
     The files required for playback are not accessible
   */
  virtual int open(std::string path,
		   std::string name,
		   uint32_t iDevice,
		   uint32_t iPort,
		   int mode,
		   uint32_t& id) = 0;
  
  /**
     Destroy a session

     The associated files will not be deleted.

     @param id
     32 bit integer with the unique value representing a recording
     session (returned by @ref create()) or a playback session
     (returned by @ref open()).

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing session
   */
  virtual int destroy(uint32_t id) = 0;

  /**
     Set whether CRC handling is enabled or not.

     When CRC handling is enabled for a recording session, the
     recorder expects all serial FPDP frames received on the
     associated port to contain a CRC word. The recorder validates the
     value of all these CRC words. If an error is detected, the
     recorder adds an CRC Error entry in the index file. The data
     associated with an incorrect CRC, is stored in the data file as
     usually.

     When CRC handling is enabled for a playback session, the recorder
     will insert a CRC in each Serial FPDP frame it transmits on the
     associated port.

     To have effect, this method must be called while the recorder
     instance is inactive (before @ref startAll is called).

     By default CRC handling is disabled both for recording and
     playback sessions.

     @param id 
     32 bit integer with the unique value representing the session
     (returned by @ref create() or @ref open()).
     @param enableCRC
     boolean value which states whether CRC checking/generation is
     enabled (true) or disabled (false)

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recorder instance
   */
  virtual int setCRC(uint32_t id,
		     bool enableCRC) = 0;

  /**
     Set whether Copy Mode is enabled or not.

     This method can only be called for recording sessions.  When
     called with the 'enable' parameter equal to true, the associated
     Serial FPDP port will start transmitting copies of all Serial
     FPDP frames it receives.

     Copy Mode and Playback are mutally exclusive. Therefore this
     method will fail if the associated Serial FPDP port is already
     used in a playback session.
     
     This method will have effect immediately when called for a
     recording session.

     @param id 
     32 bit integer with the unique value representing the recording
     session (returned by @ref create()).
     @param enable
     boolean value which states whether Copy Mode is enabled (true) 
     or disabled (false)

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recording session
     @return GE_NOT_PERMITTED
     The selected session is a playback session, or the Serial FPDP
     port of the selected session is already used by a playback
     session
   */
  virtual int setCopyMode(uint32_t id,
			  bool enable) = 0;

  /**
     Set whether or not a recording session will wait for a SYNC
     before starting recording.

     When the 'mode' parameter is @ref GEC_SFPDP_NO_WAIT_FOR_SYNC, the
     recorder starts recording on the first data received after @ref
     startAll() is called.

     When 'mode' is set to @ref GEC_SFPDP_WAIT_FOR_ANY_SYNC, the
     recorder discards any data received on the associated Serial FPDP
     port until SYNC is asserted.

     By default a recording session will use
     GEC_SFPDP_WAIT_FOR_ANY_SYNC as Wait for SYNC mode.
 
     When the recorder is waiting for SYNC, and SYNC is first asserted
     in a SYNC with Data frame, the data in that frame starts the
     recording.  But if SYNC is first asserted in a SYNC without Data
     frame, the data in the next frame starts the recording.

     This method can only be called for recording sessions (@ref
     create()). It will fail if called for playback sessions.

     To have effect, this method must be called while the recording
     session is inactive (before @ref startAll is called)

     @param id 
     32 bit integer with the unique value representing the recorder
     instance (returned by @ref create()).
     @param mode 
     integer whose value is @ref GEC_SFPDP_NO_WAIT_FOR_SYNC or @ref
     GEC_SFPDP_WAIT_FOR_ANY_SYNC
     
     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing session or the 
     value of 'mode' doesn't match a defined mode constant
     @return GE_NOT_PERMITTED
     The value of 'id' refers to a playback session
   */
  virtual int setWaitForSync(uint32_t id,
			     int mode) = 0;

  /**
     Set maximum number of bytes that can be recorded by in a
     recording session.

     This method will limit the size of the data file. It will not
     affect the size of the index file.

     If the number of recorded bytes in the data file reaches the
     limit specified by this method, further data will not be appended
     to the data file.

     The @ref stopAll() method should be called to free resources,
     even if the maximum size of the data file has been reached 
     in all recording sessions.

     @param id 
     32 bit integer with the unique value 
     representing a recording session (returned by @ref create()).
     @param sizeInBytes
     64 bit unsigned integer which specifies the maximum number of
     bytes that will be recorded in the data file

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recording session
  */
  virtual int setMaxSize(uint32_t id,
			 uint64_t sizeInBytes) = 0;

  /**
     Set the link speed to be used by a session

     There are five valid values for the 'speed' parameter.  The table
     below lists the constants that represent these values along with
     the resulting link speed


     Constant                  | Link speed
     --------------------------|------------
     @ref GEC_SFPDP_1_0625_GBPS | 1.0625 Gbps
     @ref GEC_SFPDP_2_125_GBPS  |  2.125 Gbps
     @ref GEC_SFPDP_2_5_GBPS    |    2.5 Gbps
     @ref GEC_SFPDP_3_125_GBPS  |  3.125 Gbps
     @ref GEC_SFPDP_4_25_GBPS   |   4.25 Gbps


     Note that transmit and receive on a Serial FPDP port share the
     link speed setting. Therefore playback sessions and recording
     sessions that use the same Serial FPDP port, will have the same
     link speed.

     This method must be called before the specified session is
     activated, i.e. before @ref startAll() is called. When the speed
     setting is changed, the link speed is immediately adjusted.

     By default a recorder instance will use 2.5Gbps as its link speed.

     @param id
     32 bit integer with the unique value representing the session
     (returned by @ref create() or @ref open()).
     @param speed
     integer whose value represents a predefined link speed

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing session or the
     value of 'speed' doesn't equal a defined speed constant
     @return GE_NOT_PERMITTED
     The session is already activated
  */
  virtual int setLinkSpeed(uint32_t id,
			   int speed) = 0;

  /**
     Set whether flow control is enabled or not.

     When called with the 'enable' parameter equal to true, this
     method enables flow control for the specified session.  And when
     called with the 'enable' parameter equal to false, it disables
     flow control.

     When flow control is enabled for a recording session, STOP
     ordered sets will be transmitted from the Serial FPDP port when
     the receive FIFO is almost full.

     When flow control is enabled for a playback session, the recorder
     will stop transmitting data when it receives a STOP ordered set
     from the other end. Data transmission is resumed when a GO
     ordered set is received.

     To have effect, this method must be called while the recorder
     instance is inactive (before @ref startAll is called).

     By default flow control is enabled both for recording and
     playback sessions.

     @param id 
     32 bit integer with the unique value representing the session
     (returned by @ref create() or @ref open()).
     @param enable
     boolean value which states whether flow control is enabled (true)
     or disabled (false)

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recorder instance
   */
  virtual int setFlowControl(uint32_t id,
                             bool enable) = 0;


  /**

     Set how the recorder will control the rate when transmitting data
     in a playback session.

     The recorder views data flow as a sequence of transmit cycles.
     The duration of all cycles equals the time to transmit either 512
     words, 16K words, 512K words, or 16M words. Each cycle is then
     divided in 64 parts, and the recorder is only allowed to transmit
     during a specified number of these parts at the start of each
     cycle.

     The 'cycleSize' parameter specifies the duration of the data
     cycles.  And the 'cyclePartsForTx' specifies the number of 1/64
     cycle parts during which data can be transmitted.

     To have effect, this method must be called while the recorder
     instance is inactive (before @ref startAll is called).

     By default, rate control is configured to allow transmission of
     data at full link speed ('cycleSize' is @ref
     GEC_SFPDP_16M_WORDS_CYCLE and 'cyclePartsForTx' is 64).

     @param id
     32 bit integer with the unique value representing the session
     (returned by @ref create()).
     @param cycleSize
     integer whose value is @ref GEC_SFPDP_512_WORDS_CYCLE, @ref
     GEC_SFPDP_16K_WORDS_CYCLE, @ref GEC_SFPDP_512K_WORDS_CYCLE, or @ref
     GEC_SFPDP_16M_WORDS_CYCLE.
     @param cyclePartsForTx
     32 bit unsigned integer between 1 and 64, which states how many
     1/64 parts of the transmit cycle the recorder is allowed to
     transmit data

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recorder instance
     @return GE_NOT_PERMITTED
     The value of 'id' refers to a recording session
   */
  virtual int setRateControl(uint32_t id,
                             int cycleSize,
                             uint32_t cyclePartsForTx) = 0;

  /**
     Set whether simplex link mode is enabled or disabled for a
     playback session.

     When called with the 'enable' parameter equal to 'true', this
     method enables simplex link mode for the specified session.  And
     when called with the 'enable' parameter equal to false, it
     disables simplex link mode.

     When simplex link mode is enabled, the recorder assumes that the
     playback session transmits data to a link that only supports
     outgoing signaling. There is no incoming signaling from the data
     receiver. More specifically this means that the recorder doesn't
     receive neither NRDY* signaling nor STOP/GO signaling.

     Since there is no NRDY* signaling, the recorder assumes that the
     receiver is always ready, and data transmission begins
     immediately, when the playback session is started.

     Flow control relies on STOP/GO signaling. So when this signaling
     is missing, the recorder cannot do flow control. Instead it acts
     as if GO is always signaled, and the playback session always
     transmit data at its specified rate.

     The effect of this, is that enabling simplex link mode disables
     flow control regardless of any setting made by @ref
     setFlowControl.

     To have effect, this method must be called while the recorder
     instance is inactive (before @ref startAll is called).

     By default simplex link mode is disabled for playback sessions.

     @param id
     32 bit integer with the unique value representing the session
     (returned by @ref create()).
     @param enable
     boolean value which states whether simplex link mode is enabled
     (true) or disabled (false)

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recorder instance
     @return GE_NOT_PERMITTED
     The value of 'id' refers to a recording session
   */
  virtual int setSimplexLinkMode(uint32_t id, bool enable) = 0;

  /**
     Select the source for the channel sync signal

     The recorder uses HW timers to generate the timing information
     which is stored in the metadata files during recording
     sessions. During playback sessions, the same timers can be used
     to recreate the timing of SYNC, PIO1, PIO2, and DIR signaling.

     A channel sync signal is used to synchronize the HW timers of
     different SFPDP ports.  This signal is a 5MHz clock signal which
     is either generated internally or taken from an exernal source.
     
     This method selects which channel sync source to be used by the
     session specified by the @p id argument.

     When the recorder is configured to automatically control the
     internal channel sync masters (see @ref
     setIntChannelSyncControl()), only one internal master -
     Internal Master 0 - can be selected as channel sync source. In
     addition external channal sync signals can be selected, if they
     are supported by the hardware configuration. The following table
     summarize the valid values for the @p source argument:

     Source ID                                     | Description
     ----------------------------------------------|------------:
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_0 | Internal Master 0
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT0     | EXT0 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT1     | EXT1 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT2     | EXT2 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT3     | EXT3 Coax Input

     When the recorder is configured to let the internal masters be
     controlled through the control API, all four internal masters are
     valid sources. The following table shows the IDs, that are valid
     values for the @p source argument in this mode:

     Source ID                                     | Description
     ----------------------------------------------|------------:
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_0 | Internal Master 0
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_1 | Internal Master 1
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_2 | Internal Master 2
     @ref GEC_SFPDP_INTERNAL_CHANNEL_SYNC_MASTER_3 | Internal Master 3
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT0     | EXT0 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT1     | EXT1 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT2     | EXT2 Coax Input
     @ref GEC_SFPDP_EXTERNAL_CHANNEL_SYNC_EXT3     | EXT3 Coax Input

     When the source of the channel sync signal is one of the internal
     masters, generation of the signal can either be started
     automatically by the recorder or manually through the API (see
     @ref setIntChannelSyncControl() and @ref
     setIntChannelSync()).

     Support for external channel sync signals is an optional feature
     that most SFPDP recorders do not have. On these systems without
     external channel sync signals, Internal Master 0 will usually be
     the channel sync signal source in all sessions.

     On recorders with support for external channel sync, please refer
     to the approriate ICD for details on signal routing from the
     connectors.

     By default the source of the channel sync signal is Internal
     Master 0.

     @param id
     32 bit integer with the unique value representing a recording
     session (returned by @ref create()) or a playback session
     (returned by @ref open()).
     @param source
     integer whose value is one of the 8 IDs for channel sync signal
     source

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of @p id doesn't refer to an existing session, or the
     value of @p source doesn't refer to a source
     @return GE_NOT_PERMITTED
     The source selected by @p source isn't available
   */
  virtual int setChannelSyncSource(uint32_t id, int source) = 0;

  /**
     Configure swapping inside 32 bit words.

     The SFPDP module may perform two kinds of swapping in the 32 bit
     data words being transmitted or received; 8 bit swapping inside
     16 bit and/or 16 bit swapping inside 32 bit.

     When only 8 bit swapping inside 16 bit is enabled, each 32 bit
     data word is regarded as two 16 bit fields, and the most significant
     byte and least significant byte in these 16 bit values are
     swapped. The data word 0x12345678 will for example be changed to
     0x34127856.

     When only 16 bit swapping inside 32 bit is enabled, the 16 most
     significant bits in each data word are swapped with the 16 least
     significant bits. Now the data word 0x12345678 will change to
     0x56781234.

     It is possible use 8 bit swapping inside 16 bit and 16 bit swapping
     inside 32 bit together.  This will in effect reverse the endianness
     of the received data words, ie the data word 0x12345678 will change
     to 0x78563412.

     By default both 8 bit swapping inside 16 bit and 16 bit swapping
     inside 32 bit are disabled.

     @param id
     32 bit integer with the unique value representing a recording
     session (returned by @ref create()) or a playback session
     (returned by @ref open()).
     @param enable8in16
     Boolean value specifiying whether 8 bit swapping inside 16 bit
     should be enabled (true) or disabled (false)
     @param enable16in32
     Boolean value specifiying whether 16 bit swapping inside 32 bit
     should be enabled (true) or disabled (false)

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of @p id doesn't refer to an existing session
  */
  virtual int setSwapping(uint32_t id, bool enable8in16, bool enable16in32) = 0;

  /**
     Get status of a session

     The reference parameter 'state' returns the state of a session as a string.

     A recording session may return the following state strings:

     - 'Ready' means that the session is waiting for the @ref
     startAll() method to be called.
     - 'Recording' means that the @ref startAll() method is called
     - 'Idle' means that the @ref startAll() method is called, but the
     recording session hasn't received new data the last 5 seconds.
     - 'Maxed out' means that the data file of this recording session
     has recorded the amount of data specified by @ref setMaxSize(),
     and further data is not stored.
     - 'Stopped' means that the @ref stopAll() method is called and
     further data is not recorded.

     A playback session may return the following state strings:

     - 'Ready' means that the session is waiting for the @ref
     startAll() method to be called.
     - 'Playing' means that the @ref startAll() method is called.
     - 'Stopped' means the session has stopped transmitting data
       because the @ref stopAll() method has been called.
     - 'End of file' means the session has read all data from file,
       but some of it is still waiting to be transmitted.
     - 'All data transmitted' means that the session has transmitted
       all its data.

     @param id
     32 bit integer with the unique value representing a recording
     session (returned by @ref create()) or a playback session
     (returned by @ref open()).
     @param numBytes
     64 bit unsigned integer reference returning a number of
     bytes. For a recording session this is the number of recorded
     bytes, and for a playback session this is the number of played
     bytes.
     @param state
     String reference returning the state of the session.  The
     possible states for recording sessions are 'Ready', 'Recording',
     'Idle', 'Maxed out', and 'Stopped'. And the possible states for
     playback sessions are 'Ready', 'Playing', 'Stopped', and 'End of
     file'.

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing session
   */
  virtual int getStatus(uint32_t id,
			uint64_t& numBytes,
			std::string& state) = 0;

  /**
     Check if overflow has occurred in a recording session

     The boolean reference 'detected' returns the overflow state of
     the specified session.  Its value is 'true' if one or more
     overflows has been detected since the last time the overflow
     state was cleared.

     The overflow state is cleared when recording is started (@ref
     startAll()), and when this function exits, if it is called with
     the @p clear argument set to 'true'.

     @param id
     32 bit integer with the unique value representing the recorder
     session (returned by @ref create()).
     @param detected
     boolean reference that states whether or not overflows have been detected
     @param clear
     boolean flag which states whether the overflow state should be
     cleared when this function returns

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
     @return GE_INVALID_PARAMETER
     The value of 'id' doesn't refer to an existing recording session
   */
  virtual int checkForOverflow(uint32_t id,
			       bool& detected,
			       bool clear) = 0;

  /**
     Activate all sessions

     Recording is started in all existing recording sessions, and
     playback is started in all existing playback sessions.

     @return GE_OK
     Success
     @return GEC_COMMFAIL
     Communication with remote recorder failed
   */
  virtual int startAll() = 0;

  /**
     Deactivate all sessions

     Recording is stopped in all existing recording sessions, and
     playback is stopped in all existing playback sessions.

     This method should always be called to free resources.  This
     applies even if all sessions stop by themselves (recording
     sessions because the maximum file size is reached, and playback
     sessions because they have reached the end of their data files).

     @return
     GE_OK   Success
     @return
     GEC_COMMFAIL   Communication with remote recorder failed
   */
  virtual int stopAll() = 0;
};

#endif /* GEC_ISFPDPRECORDER_H_ */
