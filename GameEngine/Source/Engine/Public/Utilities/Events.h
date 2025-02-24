#pragma once

#include <optional>
#include <functional>

template <typename... Args>
class SingleCastEvent
{
public:

	void Bind(std::function<void(Args...)> InitSubscriber)
	{
		Subscriber = std::move(InitSubscriber);
	}

	void Unbind()
	{
		Subscriber.reset();
	}

	void Execute(Args... args) const
	{
		if (Subscriber)
		{
			std::invoke(*Subscriber, std::forward<Args>(args)...);
		}
	}

private:
	std::optional<std::function<void(Args...)>> Subscriber;
};

template <typename... Args>
class MultiCastEvent
{
public:

	void Add(std::function<void(Args...)> Subscriber)
	{
		Subscribers.push_back(std::move(Subscriber));
	}

	void Remove(const std::function<void(Args...)>& Subscriber)
	{
		std::erase_if(Subscribers, [&](const auto& Predicate)
		{
			return Predicate.template target<void(Args...)>() == Subscriber.template target<void(Args...)>();
		});
	}

	void Broadcast(Args... args) const
	{
		for (const auto& Subscriber : Subscribers)
		{
			std::invoke(Subscriber, std::forward<Args>(args)...);
		}
	}

private:
	std::vector<std::function<void(Args...)>> Subscribers;
};
