// Auto-generated. Do not edit!

// (in-package diff_drive.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class GoToPoseFeedback {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.processing = null;
    }
    else {
      if (initObj.hasOwnProperty('processing')) {
        this.processing = initObj.processing
      }
      else {
        this.processing = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type GoToPoseFeedback
    // Serialize message field [processing]
    bufferOffset = _serializer.bool(obj.processing, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type GoToPoseFeedback
    let len;
    let data = new GoToPoseFeedback(null);
    // Deserialize message field [processing]
    data.processing = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'diff_drive/GoToPoseFeedback';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '14c43fbec2ed75ad470309f9d3538198';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
    # feedback
    bool processing
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new GoToPoseFeedback(null);
    if (msg.processing !== undefined) {
      resolved.processing = msg.processing;
    }
    else {
      resolved.processing = false
    }

    return resolved;
    }
};

module.exports = GoToPoseFeedback;