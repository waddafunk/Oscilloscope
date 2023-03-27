/**
 * \file InitVariables.h
 * \author Waddafunk (jacopo.piccirillo@gmail.com)
 * 
 * \version 0.2.0
 * \date 2023-03-15
 * 
 * \copyright Copyright (c) 2023
 * 
 * Init values for builder-defined parameters. THESE ARE NOT READONLY.
 * Changing these values will change the plugin inizializiation behaviour.
 * 
 */

#pragma once

/**
 * Editor initial width.
 * 
 * \return constexpr int Editor initial width. 
 */
static constexpr int EDITOR_INITIAL_WIDTH() { return 1400;};

/**
 * Editor initial heigth.
 * 
 * \return constexpr int Editor initial height.
 */
static constexpr int EDITOR_INITIAL_HEIGHT() { return 700; }

/**
 * Editor initial frame rate.
 * 
 * \return constexpr int Editor initial frame rate.
 */
static constexpr int EDITOR_INITIAL_RATE() { return 30; }

/**
 * GUI expansion animation duration.
 * 
 * \return constexpr int GUI expansion animation duration.
 */
static constexpr float GUI_EXPAND_ANIMATION_DURATION() { return 0.7; }

/**
 * Factor to multiply to PluginEditor's heigth to obtain OscilloscopeComponent
 * heigth when ControlSection is contracted.
 * 
 * \return constexpr float Factor to multiply to PluginEditor's heigth.
 */
static constexpr float GUI_CONTRACTED_MARGIN_MULTIPLIER() { return 12. / 13.; }

/**
 * Factor to multiply to PluginEditor's heigth to obtain OscilloscopeComponent
 * heigth when ControlSection is expanded.
 * 
 * \return constexpr float Factor to multiply to PluginEditor's heigth.
 */
static constexpr float GUI_EXPANDED_MARGIN_MULTIPLIER() { return 10.5 / 13.; }

/**
 * Max decay time of TriggeredOscilloscope.
 * 
 * \return constexpr float Max decay time of TriggeredOscilloscope.
 */
static constexpr float OSCILLOSCOPE_MAX_DECAY_TIME() { return 10.; }