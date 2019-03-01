#include "td-3-a.h"

Chrono::Chrono() {
  this->startTime_ = timespec_now();
  this->stopTime_ = this->startTime_;
}

void Chrono::stop() { this->stopTime_ = timespec_now(); }

void Chrono::restart() {
  this->startTime_ = timespec_now();
  this->stopTime_ = this->startTime_;
}

bool Chrono::isActive() { return this->startTime_ == this->stopTime_; }

double Chrono::startTime() { return timespec_to_ms(this->startTime_); }

double Chrono::stopTime() { return timespec_to_ms(this->stopTime_); }

double Chrono::lap() {
  timespec now = timespec_now();
  return timespec_to_ms(now - this->startTime_);
}