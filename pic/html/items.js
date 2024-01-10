const posts = [];
const images = [
  '../new/1.bmp',
  '../new/2.bmp',
  '../new/4.bmp',
  '../new/5.bmp',
  '../new/6.bmp',
  '../new/7.bmp',
  '../new/8.bmp',
  '../new/9.bmp',
  '../new/10.bmp',
  '../new/11.bmp',
  '../new/12.bmp',
  '../new/13.bmp',
  '../new/14.bmp',
  '../new/15.bmp',
  '../new/16.bmp',
  '../new/17.bmp',
  '../new/18.bmp',
  '../new/19.bmp',
  '../new/20.bmp',
  '../new/21.bmp',
  '../new/22.bmp',
  '../new/23.bmp',
  '../new/24.bmp',
  '../new/25.bmp',
  '../new/26.bmp',
  '../new/27.bmp',
  '../new/28.bmp',
  '../new/29.bmp',
  '../new/30.bmp',
  '../new/31.bmp',
];

let imageIndex = 0;

for (let i = 1; i <= 30; i++) {
  let item = {
    id: i,
    title: `Image ${i}`,
    image: images[imageIndex],
    path: `path=./pic/new/` + images[imageIndex].replace('../new/', ''),
  };
  posts.push(item);
  imageIndex++;
  if (imageIndex > images.length - 1) imageIndex = 0;
}
