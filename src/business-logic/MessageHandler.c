// #include "business-logic/MessageHandler.h"

// #include "messages/MessageTypes.h"
// #include "messages/PutKeyValueMessageRequest.h"

// void handleMessage(AugmentedBuffer* incoming_message, AugmentedBuffer* return_message)
// {
//     // Get bytes 4 through 7 and cast to message type.
//     uint32_t message_type_uint;
//     get32BitUintFromBuffer(incoming_message, 4, &message_type_uint);
//     MessageType type = (MessageType)message_type_uint;

//     switch (type)
//     {
//         case PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE:
//             PutKeyValueMessageRequest req;
//             // prepareMemForThisMes(req, )
//             // deserializePutKeyValueMessageRequest(incoming_message, &req);
//             // destoryMemForMsg
//             break;

//         case GET_VALUE_MESSAGE_REQUEST_TYPE:
//             break;

//         case GET_VALUE_MESSAGE_RESPONSE_TYPE:
//             break;
        
//         default:
//             break;
//     }
// }
