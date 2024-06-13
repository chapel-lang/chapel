(() => {
  const treeData = {
    name: "Chapel",
    children: [
      {
        name: "Desktop/Laptop",
        children: [
          {
            name: "Linux",
            classification: ["GPU"],
          },
          {
            name: "Windows (with WSL)",
            classification: ["GPU"],
          },
          {
            name: "MacOS",
          },
        ],
      },
      {
        name: "Cloud",
        children: [
          {
            name: "AWS",
            classification: ["GPU", "EFA", "Ethernet"],
          },
          {
            name: "Azure",
            classification: ["Ethernet"],
          },
        ],
      },
      {
        name: "Supercomputer",
        children: [
          {
            name: "Cray XC",
            classification: ["GPU", "Aries"],
          },
          {
            name: "HPE Apollo",
            classification: ["GPU", "InfiniBand"],
          },
          {
            name: "HPE Superdome Flex",
          },
          {
            name: "HPE Cray EX",
            classification: ["GPU", "Slingshot"],
          },
          {
            name: "Cray CS",
            classification: ["GPU", "InfiniBand"],
          },
        ],
      },
      {
        name: "Cluster",
        classification: ["GPU", "InfiniBand", "Ethernet"],
      },
      {
        name: "Container",
        children: [
          {
            name: "Docker",
          },
        ],
      },
    ],
  };

  let classifications = {
    GPU: {
      color: "#caffe5",
    },
    Slingshot: {
      color: "#a8f0e1",
    },
    Aries: {
      color: "#cbcbff",
    },
    InfiniBand: {
      color: "#FFFFCC",
    },
    EFA: {
      color: "#ffbab0",
    },
    Ethernet: {
      color: "#b9ffb9",
    },
  };


  // Only draw when the DOM is fully loaded
  window.addEventListener("DOMContentLoaded", () => {
    let rootNode = d3.select("#chapel-portability-viz");
    window.addEventListener("resize", () => {
      removeAll(rootNode);
      draw(rootNode, treeData);
    });
    draw(rootNode, treeData);
  });

  function removeAll(rootNode) {
    rootNode.selectAll("*").remove();
  }
  function draw(rootNode, treeData) {
    //
    // Create the classification dropdown
    //
    let classificationKeys = Object.keys(classifications);
    let classificationButtons = rootNode
      .append("div")
      .classed("classification-buttons", true);
    let select = classificationButtons.append("select");
    select.append("option").attr("value", "").text("Select a classification");
    for (let key of classificationKeys) {
      let option = select.append("option").attr("value", key).text(key);
    }

    let hasClassification = (d, c) => {
      return d.classification !== undefined && d.classification.includes(c);
    };
    let anyChildHasClassification = (d, c) => {
      if (d.children !== undefined) {
        for (let child of d.children) {
          if (anyChildHasClassification(child, c)) {
            return true;
          }
        }
        return false;
      }
      return hasClassification(d, c);
    };
    let allChildrenHaveClassification = (d, c) => {
      if (d.children !== undefined) {
        for (let child of d.children) {
          if (!allChildrenHaveClassification(child, c)) {
            return false;
          }
        }
        return true;
      }
      return hasClassification(d, c);
    };

    select.on("change", () => {
      let selected = select.property("value");
      nodes.each((d, i) => {
        if (selected === "") {
          rootNode.select(`#rect-${i}`).style("fill", "#FFFFFF");
          rootNode.select(`#node-${i}`).classed("highlight", false);
          return;
        }
        if (
          hasClassification(d.data, selected) ||
          anyChildHasClassification(d.data, selected)
        ) {
          rootNode
            .select(`#rect-${i}`)
            .style("fill", classifications[selected].color);
          rootNode.select(`#node-${i}`).classed("highlight", true);
        } else {
          rootNode.select(`#rect-${i}`).style("fill", "#FFFFFF");
          rootNode.select(`#node-${i}`).classed("highlight", false);
        }
      });
    });

    //
    // Create the SVG
    //

    let svg = rootNode.append("svg").classed("tree", true);
    let pathAbsBox = svg.node().getBoundingClientRect();
    let width = pathAbsBox.width;
    let height = pathAbsBox.height;
    let margin = { left: 35, top: 30, right: 35, bottom: 80 };

    let g = svg
      .append("g")
      .attr("transform", `translate(${margin.left},${margin.top})`);

    // create a layout with hierarchical data
    let treeSize = [
      width - margin.left - margin.right,
      height - margin.top - margin.bottom,
    ];
    let layout = d3
      .tree()
      .size(treeSize)
      .separation((a, b) => {
        return 100;
      });
    let root = d3.hierarchy(treeData, (d) => d.children);
    layout(root);

    let rectHeight = 30;

    // bump the y component of all the leaf nodes up or down
    let bumpY = 10;
    root
      .descendants()
      .slice(1)
      .forEach((d, i) => {
        if (i % 3 === 0) {
          d.y += rectHeight + bumpY;
        } else if (i % 3 === 1) {
          d.y -= rectHeight + bumpY;
        }
      });
    // shrink the graph up slightly
    root
      .descendants()
      .slice(1)
      .forEach((d, i) => {
        d.y -= d.depth ** 2 * 20;
      });

    g.selectAll("path")
      // ignore root
      .data(root.descendants().slice(1))
      .enter()
      .append("path")
      .attr("d", (d) => {
        return d3.linkVertical()({
          source: [d.x, d.y],
          target: [d.parent.x, d.parent.y],
        });
      })
      .style("fill", "none")
      .attr("stroke", "#000000")
      .attr("stroke-width", 2)
      .exit();

    let nodes = g
      .selectAll("g")
      .data(root.descendants())
      .enter()
      .append("g")
      .attr("id", (d, i) => `node-${i}`);

    nodes
      .append("text")
      .text((d) => d.data.name)
      .attr("x", (d) => d.x)
      .attr("y", (d) => d.y)
      .attr("text-anchor", "middle")
      .each((d, i, nodes) => {
        d.data.width = nodes[i].getComputedTextLength();
      });

    let rectColor = "#4CAF50";
    let dimRectColor = "#5cb860";
    let clickedRectColor = "#91cf94";

    nodes
      .append("rect")
      .attr("id", (d, i) => `rect-${i}`)
      .attr("x", (d) => d.x - (d.data.width + 10) / 2)
      .attr("y", (d) => d.y - rectHeight / 2 - 5)
      .attr("width", (d) => d.data.width + 10)
      .attr("height", rectHeight)
      .attr("rx", 5)
      .attr("ry", 5)
      .style("fill", (d) => {
        if (d.data.popupHtml !== undefined) {
          return rectColor;
        } else {
          return "#FFFFFF";
        }
      })
      .attr("stroke", "grey")
      .style("stroke-width", 1)
      .style("stroke-opacity", "0.5")
      .order()
      .lower();

    //
    // Configure the tooltip
    // Note, this code currently will do nothing as the treeData does not have any popupHtml
    //

    let tooltip = rootNode
      .append("div")
      .classed("tooltip", true)
      .classed("hidden", true)
      .style("opacity", "0");
    let clickedTooltip = null;

    let updateTooltip = (d, i) => {
      rootNode.select(`#rect-${i}`).style("fill", dimRectColor);
      tooltip
        .classed("hidden", false)
        .transition()
        .duration(50)
        .style("opacity", "1");
      let tooltipText = `${d.data.popupHtml}`;
      tooltip.html(tooltipText);
      //move tool tip
      let tool_height = +tooltip.style("height").replace("px", "");
      tooltip
        .style("left", d3.event.pageX + 15 + "px")
        .style("top", d3.event.pageY - tool_height + "px");
    };
    let removeTooltip = (d, i) => {
      rootNode.select(`#rect-${i}`).style("fill", rectColor);
      tooltip
        .classed("hidden", true)
        .transition()
        .duration(50)
        .style("opacity", "0");
    };

    nodes.on("mouseover", (d, i) => {
      if (d.data.popupHtml === undefined) return;
      if (clickedTooltip !== null) return;
      updateTooltip(d, i);
    });
    nodes.on("mousemove", (d, i) => {
      if (d.data.popupHtml === undefined) return;
      if (clickedTooltip !== null) return;
      updateTooltip(d, i);
    });
    nodes.on("mouseout", (d, i) => {
      if (d.data.popupHtml === undefined) return;
      if (clickedTooltip !== null) return;
      removeTooltip(d, i);
    });
    nodes.on("click", (d, i) => {
      if (d.data.popupHtml === undefined) return;

      // if the same rect is clicked again, close the tooltip
      if (clickedTooltip === i) {
        rootNode.select(`#rect-${clickedTooltip}`).style("fill", rectColor);
        tooltip
          .classed("hidden", true)
          .transition()
          .duration(50)
          .style("opacity", "0");
        clickedTooltip = null;
        removeTooltip(d, i);
      } else if (clickedTooltip !== null) {
        rootNode.select(`#rect-${clickedTooltip}`).style("fill", rectColor);
        clickedTooltip = i;
        updateTooltip(d, i);
        rootNode.select(`#rect-${i}`).style("fill", clickedRectColor);
      } else {
        clickedTooltip = i;
        updateTooltip(d, i);
        rootNode.select(`#rect-${i}`).style("fill", clickedRectColor);
      }
    });
  }
})();
