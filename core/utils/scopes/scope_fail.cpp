#include "scope_fail.hpp"

#include <exception>

ScopeFail::ScopeFail(std::function<void(const std::string&)> fn) : fn_(std::move(fn)) {}

ScopeFail::~ScopeFail() {
    if (std::uncaught_exceptions() > 0) {
        std::exception_ptr currentException = std::current_exception();
        if (currentException) {
            try {
                std::rethrow_exception(currentException);
            } catch (const std::exception& exception) {
                fn_(exception.what());
            } catch (...) {
                fn_("unknown non-standard error");
            }
        }
    }
}
