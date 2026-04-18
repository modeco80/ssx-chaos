
template<typename Ret>
inline HookHandle safeTrampoline(Ret(*pTarget)(), Ret(*pHook)(), Ret(**ppTrampoline)()) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }

template<typename Ret, typename A1>
inline HookHandle safeTrampoline(Ret(*pTarget)(A1), Ret(*pHook)(A1), Ret(**ppTrampoline)(A1)) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }

template<typename Ret, typename A1, typename A2>
inline HookHandle safeTrampoline(Ret(*pTarget)(A1, A2), Ret(*pHook)(A1, A2), Ret(**ppTrampoline)(A1, A2)) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }

template<typename Ret, typename A1, typename A2, typename A3>
inline HookHandle safeTrampoline(Ret(*pTarget)(A1, A2, A3), Ret(*pHook)(A1, A2, A3), Ret(**ppTrampoline)(A1, A2, A3)) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }

template<typename Ret, typename A1, typename A2, typename A3, typename A4>
inline HookHandle safeTrampoline(Ret(*pTarget)(A1, A2, A3, A4), Ret(*pHook)(A1, A2, A3, A4), Ret(**ppTrampoline)(A1, A2, A3, A4)) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }

template<typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
inline HookHandle safeTrampoline(Ret(*pTarget)(A1, A2, A3, A4, A5), Ret(*pHook)(A1, A2, A3, A4, A5), Ret(**ppTrampoline)(A1, A2, A3, A4, A5)) { return trampolineHook(reinterpret_cast<void*>(pTarget), reinterpret_cast<void*>(pHook), reinterpret_cast<void**>(ppTrampoline)); }
