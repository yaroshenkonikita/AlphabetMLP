#include "graph_mse_window.h"

#include <iostream>

#include "ui_graph_mse_window.h"

namespace s21 {

namespace {

std::vector<QColor> colours_ = {QColor("cyan"),     QColor("magenta"),
                                QColor("red"),      QColor("darkRed"),
                                QColor("darkCyan"), QColor("darkMagenta"),
                                QColor("green"),    QColor("darkGreen"),
                                QColor("yellow"),   QColor("blue")};
}  // namespace

GraphMseWindow::GraphMseWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GraphMseWindow),
      max_count_(0),
      max_value_(0.f) {
  for (auto &color : colours_) {
    color.setAlpha(150);
  }
  ui->setupUi(this);
  ui->graph->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->graph->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

  ui->graph->xAxis->setLabel("Times");
  ui->graph->yAxis->setLabel("Error");

  ui->graph->yAxis2->setVisible(true);
  ui->graph->yAxis2->setTicks(false);
  ui->graph->yAxis2->setTickLabels(false);

  ui->graph->xAxis->setRange(0, 1000);
  ui->graph->yAxis->setRange(0, max_value_);

  ui->graph->legend->setVisible(true);
  ui->graph->legend->setBrush(QColor(255, 255, 255, 150));
  ui->graph->setInteraction(QCP::iRangeDrag, true);
  ui->graph->setInteraction(QCP::iRangeZoom, true);
}

GraphMseWindow::~GraphMseWindow() { delete ui; }

void GraphMseWindow::AddGraph(std::vector<double> mse) {
  if (mse.empty()) {
    return;
  }
  if (ui->graph->graphCount() == static_cast<int>(colours_.size())) {
    ClearGraph();
  }
  QColor color = colours_[ui->graph->graphCount()];
  ui->graph->addGraph();
  ui->graph->graph()->setLineStyle(QCPGraph::lsLine);
  ui->graph->graph()->setPen(QPen(color.lighter(200)));
  ui->graph->graph()->setBrush(QBrush(color));
  auto every = static_cast<std::size_t>(sqrt(static_cast<double>(mse.size())));
  if (mse.size() > 4lu) {
    every *= 2lu;
  }
  std::size_t new_size = mse.size() / every;
  QVector<QCPGraphData> graphData(static_cast<QVector<QCPGraphData>::size_type>(
      new_size == 0 ? mse.size() : new_size));
  double mse_value = 0.f;
  for (int64_t i = 0; i < graphData.size(); ++i) {
    graphData[i].key = static_cast<double>(i * every);
    double value = 0;
    for (std::size_t index = i * every; index < (i + 1) * every; ++index) {
      value += mse[index];
    }
    mse_value += value;
    graphData[i].value = value / static_cast<double>(every);
    max_value_ = std::max(max_value_, static_cast<float>(graphData[i].value));
  }
  mse_value /= static_cast<double>(mse.size());
  ui->graph->graph()->data()->set(graphData);
  max_count_ = std::max(max_count_, mse.size());
  ui->graph->xAxis->setRange(0, static_cast<double>(max_count_));
  ui->graph->yAxis->setRange(0, max_value_);
  ui->graph->replot();
  ui->graph->graph()->setName("MSE " +
                              QString::number(ui->graph->graphCount()) + " - " +
                              QString::number(mse_value, 'f', 4));
}

void GraphMseWindow::ClearGraph() { ui->graph->clearGraphs(); }

}  // namespace s21
