/*
  NAME:
  gbj_appdevice

  DESCRIPTION:
  Application library for processing and publishing categorized data items
  to IoT platform.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
 */
#ifndef GBJ_APPDEVICE_H
#define GBJ_APPDEVICE_H

#if defined(__AVR__)
  #include <Arduino.h>
#elif defined(ESP8266)
  #include <Arduino.h>
#elif defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_appdata.h"
#include "gbj_appthingsboard.h"
#include "gbj_serial_debug.h"

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "gbj_appdevice"

class gbj_appdevice : public gbj_appthingsboard
{
public:
  const char *VERSION = "GBJ_APPDEVICE 1.0.0";

  /*
    Constructor.

    DESCRIPTION:
    Constructor creates the class instance object and sets credentials for
    IoT platform.

    PARAMETERS:
    server - IP address of a ThingsBoard server.
      - Data type: constant string
      - Default value: none
      - Limited range: none
    token - Device authorization token.
      - Data type: constant string
      - Default value: none
      - Limited range: none
    handlers - A structure with pointers to various callback handler
    functions.
      - Data type: Handlers
      - Default value: structure with zeroed all handlers
      - Limited range: system address range

    RETURN: object
  */
  inline gbj_appdevice(const char *server,
                       const char *token,
                       Handlers handlers = Handlers())
    : gbj_appthingsboard(server, token, handlers)
  {
  }
  /*
    Initialization.

    DESCRIPTION:
    The method should be called in the SETUP section of a sketch.

    PARAMETERS:
    datahub - A pointer to the instance object for data hub.
      - Data type: gbj_datahub
      - Default value: none
      - Limited range: system address space

    RETURN: Result code.
  */
  void begin(gbj_appdata *datahub)
  {
    SERIAL_TITLE("begin")
    datahub_ = datahub;
    // Init data items
    initStatics();
    initParams();
    initTelemetry();
    initEvents();
  }

  // Statics (attributes) - constants published at boot
  inline void publishAttribsStatic()
  {
    for (gbj_appdata::Parameter *prm : ptrStatics_)
    {
      processAttrib(prm);
    }
  }
  // Parameters (attributes) - mostly from EEPROM; changed occassionally
  inline void publishAttribsDynamic()
  {
    for (gbj_appdata::Parameter *prm : ptrParams_)
    {
      processAttrib(prm);
    }
  }
  // Events (measures) - published at change; changed occassionally
  inline void publishEvents()
  {
    for (gbj_appdata::Parameter *prm : ptrEvents_)
    {
      processMeasure(prm);
    }
  }
  // Telemetry (measures) - published periodically; changed frequently
  inline void publishMeasures()
  {
    for (gbj_appdata::Parameter *prm : ptrTelemetry_)
    {
      processMeasure(prm);
    }
  }

protected:
  std::vector<gbj_appdata::Parameter *> ptrStatics_, ptrParams_, ptrTelemetry_,
    ptrEvents_;

  // Abstract methods
  virtual void initStatics() = 0;
  virtual void initParams() = 0;
  virtual void initTelemetry() = 0;
  virtual void initEvents() = 0;

private:
  gbj_appdata *datahub_;
  inline void processAttrib(gbj_appdata::Parameter *prm)
  {
    if (prm->isReady())
    {
      publishAttrib(datahub_->getPrmName(prm->getName()),
                    prm->publish().c_str());
      isError() ? prm->init() : prm->reset();
    }
  }

  inline void processMeasure(gbj_appdata::Parameter *prm)
  {
    if (prm->isReady())
    {
      publishMeasure(datahub_->getPrmName(prm->getName()),
                     prm->publish().c_str());
      isError() ? prm->init() : prm->reset();
    }
  }
};

#endif