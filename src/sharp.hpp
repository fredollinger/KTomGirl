#ifndef _SHARP_HPP__
#define _SHARP_HPP__

/* sharp
 * drop-in replacement for all things sharp
 */

#include <QDir>
#include <QDateTime>
#include <string>

namespace sharp{

bool directory_exists(const std::string & directory);
} // namespace sharp 
#endif
