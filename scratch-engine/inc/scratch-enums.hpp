/*
File: scratch-enums.hpp
Description: Contains all enum types that CScratch uses
*/
#pragma once
namespace scratch
{
    enum class engine_status
    {
        ok = 0,
        error = 1,
        exited = 2
    };
    enum class state_type
    {
        number = 0,
        boolean = 1,
        color = 2,
        string = 3
    };
    enum class job_status
    {
        ok = 0,
        error = 1,
        signal_engine_terminate = 2, // need this as input job can tell scratch engine to exit
        signal_job_terminate = 3, // signal for jobs to tell scheduler to stop running the job in the future
        signal_scheduler_terminate_others = 4, // need this to emulate stop other scripts in sprite
        signal_vm_halt = 5, // need this to simulate red stop button and stop all scripts
    };
    enum class core_jobs
    {
        input = 0,
        render = 1
    };
    enum class input_state : unsigned char
    {
        fresh_pressed = 2,
        pressed = 1,
        unpressed = 0
    };
    enum class rotation_mode
    {
        all_around = 0,
        left_right = 1,
        none = 2
    };
    enum class graphical_effect
    {
        color = 0,
        fisheye = 1,
        whirl = 2,
        pixelate = 3,
        mosaic = 4,
        brightness = 5,
        ghost = 6,
        max = 7
    };
}