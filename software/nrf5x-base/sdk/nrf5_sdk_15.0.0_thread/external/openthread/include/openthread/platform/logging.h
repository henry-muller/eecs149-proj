/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the debug log service.
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdarg.h>
#include <stdint.h>

#include <openthread/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup plat-logging
 *
 * @brief
 *   This module includes the platform abstraction for the debug log service.
 *
 * @{
 *
 */

/**
 * Log levels.
 *
 * Implementation note: These exist as defines (not enums) so that
 * embedded code can remove code at compile time via if/else/endif
 */

#define OT_LOG_LEVEL_NONE 0 ///< None
#define OT_LOG_LEVEL_CRIT 1 ///< Critical
#define OT_LOG_LEVEL_WARN 2 ///< Warning
#define OT_LOG_LEVEL_INFO 3 ///< Info
#define OT_LOG_LEVEL_DEBG 4 ///< Debug

/* note: The enum otLogRegion is found in 'openthread/types.h' */

/**
 * This function outputs logs.
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aLogRegion  The log region.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ...         Arguments for the format specification.
 *
 */
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...);

/**
 * This function outputs logs.
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aLogRegion  The log region.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ap          va_list matching information for aFormat
 *
 */
void otPlatLogv(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, va_list ap);

/**
 * @}
 *
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // DEBUG_H_
