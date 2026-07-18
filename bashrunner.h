#pragma once

#include <KRunner/AbstractRunner>

class BashRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    BashRunner(QObject *parent, const KPluginMetaData &data);

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
    void reloadConfiguration() override;

protected:
    void init() override;

private:
    QString m_triggerWord;
};