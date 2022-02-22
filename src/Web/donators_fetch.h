#pragma once
#include <vector>
#include <string>

const std::vector<std::string>& GetDonatorNames();
const std::vector<int>& GetDonatorTiers();
void StartAsyncDonatorsFetch();
