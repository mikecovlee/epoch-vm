#pragma once

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include "any.hpp"

namespace epoch
{
    template<typename KeyT, typename ValT> using map = std::unordered_map<KeyT, ValT>;
    using string = std::string;
    using size_t = std::size_t;
    namespace vm_types {
        using vm_int = std::int32_t;
        using vm_uint = std::uint32_t;
        using vm_char = std::int8_t;
        using vm_bool = std::int8_t;
        using vm_float = double;
        using vm_string = std::string;
        using vm_any = any;
    }

    template<typename ...> struct sizeof_max;

    template<typename T> struct sizeof_max<T>
    {
        static constexpr std::size_t get(std::size_t size = 0)
        {
            if (sizeof(T) > size)
                return sizeof(T);
            else
                return size;
        }
    };

    template<typename T, typename ...ArgsT> struct sizeof_max<T, ArgsT...>
    {
        static constexpr std::size_t get(std::size_t size = 0)
        {
            if (sizeof(T) > size)
                return sizeof_max<ArgsT...>::get(sizeof(T));
            else
                return sizeof_max<ArgsT...>::get(size);
        }
    };


    class vm_object {
    public:
        enum class types
        {
            vm_null, vm_int, vm_uint, vm_char, vm_bool, vm_float, vm_string, vm_any
        };
    private:
        std::uint8_t data[sizeof_max<vm_types::vm_int, vm_types::vm_uint, vm_types::vm_char, vm_types::vm_bool, vm_types::vm_float, vm_types::vm_string, vm_types::vm_any>::get()];
        types type = types::vm_null;
    public:
        bool is_primitive() const
        {
            return type != types::vm_null && type != types::vm_string && type != types::vm_any;
        }
        bool is_signed() const
        {
            return is_primitive() && type != types::vm_uint;
        }
    };

    class vm_instance final {
        struct stack_frame {
            map<string, size_t>  local_variables;
            stack_type<any>      local_slots;
            stack_type<any_view> local_stack;
            stack_frame(size_t local_s, size_t frame_s) : local_slots(local_s), local_stack(frame_s) {}
        };
        stack_type<stack_frame> stack;
        size_t stack_size, local_size, frame_size;
    public:
        vm_instance() = delete;
        vm_instance(const vm_instance&) = delete;
        vm_instance(size_t stack_s, size_t local_s, size_t frame_s) : stack(stack_s), stack_size(stack_s), local_size(local_s), frame_size(frame_s) {}
        
        // Stack Frame
        inline void push_frame();

        inline void pop_frame();

        // Variable
        size_t add_variable(const string &, const any &);

        void set_variable_field(map<string, size_t>, std::vector<any>);

        inline void load_variable(size_t);

        inline void load_variable(const string &);

        void search_variable_dyn(const string &);

        // Immediate Value
        inline void push_int(types::vm_int);

        inline void push_uint(types::vm_uint);

        inline void push_char(types::vm_char);

        inline void push_bool(types::vm_bool);

        inline void push_float(types::vm_float);

        inline void push_string(types::vm_string);

        // Calculate
        void cal_add();

        void cal_sub();

        void cal_mul();

        void cal_div();
    };
    class vm_instance_builder final {

    };
}