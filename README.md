<a id="library"></a>

# gbj\_appdevice
This is an application library, which is used for derivating project specific libraries. So that it can be considered as a helper library between a project specific library and the application library `gbj_appthingsboard`. The encapsulation provides following advantages:

* Functionality is hidden from the main sketch.
* The library follows the principle `separation of concerns`.
* The library is reusable for various projects without need to code processing and preparation data.
* Update in library is valid for all involved projects.
* It specifies (inherits from) the parent application library `gbj_appthingsboard`.
* It utilizes funcionality and error handling from the parent class.


## Fundamental functionality
* The library is aimed to process and prepared project data items aimed and categorized for publishing in <abbr title='Internet Of Things'>IoT</abbr> platform in uniform way* The library utilizes internal timer for periodical data publishing to IoT platform.
* The library, meaning the class defined in it, implements virtual methods from its parent library and hides them from corresponding project specific library.
* On the other hand, the library declares another virtual methods for categorizing published data items, which derived project specific libary has to implement.
* The class from the library is not intended to be used directly in a sketch, just as a parent class for project specific device libraries defining and categozing published data items, e.g., `apptb_device`.
* The methods, custom types, and other features not descibed in this document are valid and descibed in the documentation for the parent class.


<a id="dependency"></a>

## Dependency
* **gbj\_appdata**: Parent library with definition the data hub with published items loaded from the file `gbj_appdata.h`.
* **gbj\_appthingsboard**: Parent library for connection and communication with IoT platform [ThingsBoard](https://thingsboard.io/docs/getting-started-guides/what-is-thingsboard/) loaded from the file `gbj_appthingsboard.h`.
* **gbj\_serial\_debug**: Auxilliary library for debug serial output loaded from the file `gbj_serial_debug.h`. It enables to exclude serial outputs from final compilation.
* **Arduino.h**: Main include file for the Arduino platform.


<a id="interface"></a>

## Interface
The methods in bold are virtual methods and should be implemented in a project specific library.

* [gbj_appdevice()](#gbj_appdevice)
* [begin()](#begin)
* [**initStatics()**](#init)
* [**initParams()**](#init)
* [**initTelemetry()**](#init)
* [**initEvents()**](#init)


<a id="gbj_appdevice"></a>

## gbj_appdevice()

#### Description
Constructor creates the class instance object and initiates internal resources.
* It inputs credentials for identifying an IoT Platform device as well as pointer to an external structure with handlers.
* All input arguments are passed to the constructor of the parent class without any change.

#### Syntax
    gbj_appdevice(const char *server, const char *token, Handlers handlers)

#### Parameters
* **server**: Pointer to an address of ThingsBoard server. It should be either IP address or web address.
  * *Valid values*: constant pointer to string
  * *Default value*: none


* **token**: Pointer to the authorization token of a ThingsBoard device.
  * *Valid values*: constant pointer to string
  * *Default value*: none


* **handlers**: Pointer to a structure of callback functions. This structure as well as handlers should be defined in the main sketch.
  * *Data type*: Handlers
  * *Default value*: empty structure

#### Returns
Object performing processing published data items.

[Back to interface](#interface)


<a id="begin"></a>

## begin()

#### Description
The method initializes published data items by calling all virtual methods implemented in the child class with data from reference data hub.

#### Syntax
    void begin(gbj_appdata *datahub)

#### Parameters
* **datahub**: Pointer to an external structure of parameters to be published in IoT platform.
  * *Valid values*: pointer to an object
  * *Default value*: none

#### Returns
None

[Back to interface](#interface)


<a id="publishMeasure"></a>

## initStatics(), initParams(), initTelemetry(), initEvents()

#### Description
The methods are virutal methods, which should be implemented in a child project specific library.

* **initStatics()** - Method initializes static attributes just after the booting a microcontroller. They are never changed during the run of it.

* **initParams()** - Method initializes dynamic attributes that act as parameters change occassionally and rarely. They are published right after the change without waiting for next regular publishing period.

* **initTelemetry()** - Method initializes telemetry data items that act as timeseries or measures. They are published regularly in regular time publishing periods regardless the change time within a period. The publishing can occur either after a change of the data items or everytime regardless of changing their value.

* **initEvents()** - Method initializes telemetry data items that act as occassionally changed timeseries or measures. They are published right after the change outside the regular publishing periods, but only at change their value.

[Back to interface](#interface)
