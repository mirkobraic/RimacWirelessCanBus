#ifndef UDSCONSTANTSUNPACKER_H
#define UDSCONSTANTSUNPACKER_H

#include <QString>
#include "uds_client_api/client_services_structures.hpp"

class UdsConstantsUnpacker
{
public:
    UdsConstantsUnpacker();

    static QString unpackNegativeResponse(uds::response::negative_response res);
    static QString unpackResponseError(uds::response::response_error res);
};

#endif // UDSCONSTANTSUNPACKER_H
