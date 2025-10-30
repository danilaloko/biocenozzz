#pragma once

#include <QUuid>
#include <functional>

namespace std {
    template<>
    struct hash<QUuid> {
        std::size_t operator()(const QUuid& uuid) const noexcept {
            return ::qHash(uuid);
        }
    };
}